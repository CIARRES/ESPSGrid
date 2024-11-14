#include "powerMeter.h"

#include <iostream>
#include <unistd.h>

PowerMeter* powerMeter;

/**
 * @brief Signal handler for SIGINT to stop the server.
 * 
 * @param signalId The signal identifier.
 */
void sigint_handler(int signalId)
{
    printf("Stopping Power Meter...\n\n");
    powerMeter->stop();
}

/***********************************************
 *                                             *
 *             Wrapper Functions               *
 *                                             *
 ***********************************************/

void
*execDisplayData(void *args)
{
    PowerMeter* pm = (PowerMeter*)args;
    pm->displayData();
    return NULL;
}

void
*execSecurityMonitoring(void *args)
{
    PowerMeter* pm = (PowerMeter*)args;
    pm->securityMonitoring();
    return NULL;
}

/***********************************************
 *                                             *
 *          Power Meter Functions              *
 *                                             *
 ***********************************************/

PowerMeter::PowerMeter(int16_t id, char* ip, int16_t dataPort, char* gooseInterface, int16_t safetyThreshold)
{
    this->id = id;

    this->ip = (char*)malloc(IP_SIZE * sizeof(char));
    strncpy(this->ip, ip, IP_SIZE);

    this->dataPort = dataPort;

    this->gooseInterface = (char*)malloc(INTERFACE_SIZE * sizeof(char));
    if (gooseInterface != NULL)
    {
        strncpy(this->gooseInterface, gooseInterface, INTERFACE_SIZE);
    }
    else
        strncpy(this->gooseInterface, DEF_GOOSE_INTERFACE, INTERFACE_SIZE);

    this->model = new PowerMeterModel(id, safetyThreshold);

    this->server = new IEC61850Server(MMS_PORT, this->gooseInterface, model);
}

SimLinkModel*
PowerMeter::createSimlinkModel()
{
    SimLinkModel* simModel = (SimLinkModel*)malloc(sizeof(SimLinkModel));

    strncpy(simModel->simulinkIp, ip, IP_SIZE);
    simModel->numStations = 1;
    simModel->commDelay = 250;

    // Create station
    simModel->stationsInfo = (StationInfo *)malloc(simModel->numStations * sizeof(StationInfo));
    simModel->stationsInfo->genericInPorts[0] = dataPort;

    // Create data buffers
    simModel->stationsData = (StationData *)malloc(simModel->numStations * sizeof(StationData));
    simModel->stationsData->genericIn[0].count = 2 * THREE_PHASES;
    simModel->stationsData->genericIn[0].itemSize = sizeof(double);
    simModel->stationsData->genericIn[0].maxSize = 2 * THREE_PHASES * sizeof(double);
    simModel->stationsData->genericIn[0].data = (double *)calloc(2 * THREE_PHASES, sizeof(double));

    // Link data buffers
    model->data.voltages = (double*)simModel->stationsData->genericIn[0].data;
    model->data.currents = (double*)(void *)((char *)simModel->stationsData->genericIn[0].data + THREE_PHASES * sizeof(double));

    // Create and link mutex
    pthread_mutex_init(&simModel->lock, NULL);
    model->data.dataLock = &simModel->lock;

    return simModel;
}

void 
PowerMeter::displayData()
{
    char pmName[10];
    snprintf(pmName, 10, "PM%02d", id);

    while(model->running)
    {
        pthread_mutex_lock(model->data.dataLock);
        int16_t safe = model->data.safeState;
        double vA = model->data.voltages[0];
        double vB = model->data.voltages[1];
        double vC = model->data.voltages[2];
        double iA = model->data.currents[0];
        double iB = model->data.currents[1];
        double iC = model->data.currents[2];
        pthread_mutex_unlock(model->data.dataLock);

        printf("%s:\n", pmName);
        printf("\tVoltage: %f\t\t%f\t\t%f\n", vA, vB, vC);
        printf("\tCurrent: %f\t\t%f\t\t%f\n", iA, iB, iC);
        printf("\tSafe: %d\n", safe);

        usleep(2000000);
    }
}

void
PowerMeter::setUp()
{
    signal(SIGINT, sigint_handler);

    // Set up the Simulink connection
    SimLinkModel* simLinkModel = createSimlinkModel();

    simLink = new SimLink(simLinkModel);

    // Start the IEC61850 server
    model->running = server->startServer();
}

void
PowerMeter::securityMonitoring()
{
    int safetyCounter = 0;

    /* Monitor security state */
    while(model->running)
    {
        pthread_mutex_lock(model->data.dataLock);
        double iA = model->data.currents[0];
        double iB = model->data.currents[1];
        double iC = model->data.currents[2];
        pthread_mutex_unlock(model->data.dataLock);

        bool newState = !(iA > model->safetyThreshold || iB > model->safetyThreshold || iC > model->safetyThreshold);

        if (!model->data.safeState && newState && safetyCounter < 3)
        {
            safetyCounter++;
        }
        else
        {
            pthread_mutex_unlock(model->data.dataLock);
            model->data.safeState = newState;
            pthread_mutex_unlock(model->data.dataLock);
            safetyCounter = 0;
        }

        // In case of unsafe circuit, wait for 3s to stabilize
        if (!model->data.safeState)
            usleep(3000);

        usleep(1000);
    }
}

void 
PowerMeter::run()
{
    server->startDataExchange();
    simLink->exchangeDataWithSimulink();

    pthread_t displayThread;
    pthread_create(&displayThread, NULL, execDisplayData, (void*)this);

    pthread_t securityThread;
    pthread_create(&securityThread, NULL, execSecurityMonitoring, (void*)this);

    while (model->running){}

}

void
PowerMeter::stop()
{
    model->running = false;
    server->stopServer();
}

PowerMeter::~PowerMeter()
{
    if (server != nullptr) {
        delete server;
        server = nullptr;
    }

    if (simLink != nullptr) {
        delete simLink;
        simLink = nullptr;
    }

    if (model != nullptr) {
        delete model;
        model = nullptr;
    }
}

/***********************************************
 *                                             *
 *               Main Function                 *
 *                                             *
 ***********************************************/

/**
 * @brief Main function to initialize and run the power meter model and MMS server.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return int The exit status.
 */
int main(int argc, char* argv[]) {

    /* Create model and parse parameters */

    if (argc < 5 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " <PM_ID> <Simulink_IP> <DataPort> <safe_threshold> [<GOOSE_interface>]" << std::endl;
        return 1;
    }

    int16_t id = (int16_t)atoi(argv[1]);
    char* ip = argv[2];

    int16_t dataPort = (int16_t)atoi(argv[3]);

    int16_t safetyThreshold = (int16_t)atoi(argv[4]);
    char* goose_interface = NULL;

    if (argc == 6)
        goose_interface = argv[5];

    /* Create PowerMeter */

    printf("Creating PowerMeter...\n\n");
    powerMeter = new PowerMeter(id, ip, dataPort, goose_interface, safetyThreshold);

    /* SetUp PowerMeter */

    printf("Setting up PowerMeter...\n\n");
    powerMeter->setUp();

    printf("Running PowerMeter...\n\n");
    powerMeter->run();

    //powerMeter->destroy();

    return 0;
}