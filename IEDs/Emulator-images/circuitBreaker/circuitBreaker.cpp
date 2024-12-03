#include "circuitBreaker.h"

#include <iostream>
#include <unistd.h>

CircuitBreaker* circuitBreaker;

/**
 * @brief Signal handler for SIGINT to stop the server.
 * 
 * @param signalId The signal identifier.
 */
void sigint_handler(int signalId)
{
    printf("Stopping Circuit Breaker...\n\n");
    circuitBreaker->stop();
}

/***********************************************
 *                                             *
 *             Wrapper Functions               *
 *                                             *
 ***********************************************/

void
*execDisplayData(void *args)
{
    CircuitBreaker* cb = (CircuitBreaker*)args;
    cb->displayData();
    return NULL;
}

/***********************************************
 *                                             *
 *         Circuit Breaker Functions           *
 *                                             *
 ***********************************************/

CircuitBreaker::CircuitBreaker(int16_t id, char* ip, int16_t dataPort, char* goose_interface, bool automaticOperationMode)
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

    this->model = (CircuitBreakerModel*)malloc(sizeof(CircuitBreakerModel));
    model->id = id;
    model->data.dataLock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    model->data.safeState = true;
    model->data.operatorState = false;
    model->data.currentState = NULL;
    model->data.automaticOperationMode = automaticOperationMode;

    this->server = new IEC61850Server(MMS_PORT, this->gooseInterface, model);
}

void 
CircuitBreaker::displayData()
{
    char cbName[10];
    snprintf(cbName, 10, "CB%02d", id);

    while(model->running)
    {
        pthread_mutex_lock(model->data.dataLock);
        bool safeState = model->data.safeState;
        bool operatorState = model->data.operatorState;
        bool breakerState = model->data.currentState;
        pthread_mutex_unlock(model->data.dataLock);

        printf("%s:\n", cbName);
        printf("\tSecurity: %d\n", safeState);
        printf("\tOperator: %d\n", operatorState);
        printf("\tBreaker: %d\n", breakerState);

        usleep(2000000);
    }
}

SimLinkModel*
CircuitBreaker::createSimlinkModel()
{
    SimLinkModel* simModel = (SimLinkModel*)malloc(sizeof(SimLinkModel));

    strncpy(simModel->simulinkIp, ip, IP_SIZE);
    simModel->numStations = 1;
    simModel->commDelay = 250;

    // Create station
    simModel->stationsInfo = (StationInfo *)malloc(simModel->numStations * sizeof(StationInfo));
    simModel->stationsInfo->digitalOutPorts[0] = dataPort;

    // Create data buffers
    simModel->stationsData = (StationData *)malloc(simModel->numStations * sizeof(StationData));
    simModel->stationsData->digitalOut[0] = automaticOperationMode;

    // Link data buffers
    model->data.currentState = (bool*)simModel->stationsData->digitalOut;

    // Create and link mutex
    pthread_mutex_init(&simModel->lock, NULL);
    model->data.dataLock = &simModel->lock;

    return simModel;
}

void
CircuitBreaker::setUp()
{
    signal(SIGINT, sigint_handler);

    // Set up the Simulink connection
    SimLinkModel* simLinkModel = createSimlinkModel();

    simLink = new SimLink(simLinkModel);

    // Start the IEC61850 server
    model->running = server->startServer();
}

void 
CircuitBreaker::run()
{
    server->startDataExchange();
    simLink->exchangeDataWithSimulink();

    pthread_t displayThread;
    pthread_create(&displayThread, NULL, execDisplayData, (void*)this);

    while (model->running){}
}

void
CircuitBreaker::stop()
{
    model->running = false;
    server->stopServer();
}

CircuitBreaker::~CircuitBreaker()
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
 * @brief Main function to initialize and run the smart meter model and MMS server.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return int The exit status.
 */
int main(int argc, char* argv[]) {

    /* Create model and parse parameters */

    if (argc < 5 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " <CB_ID> <Simulink_IP> <DataPort> <automatic_mode> [<GOOSE_interface>]" << std::endl;
        return 1;
    }

    int16_t id = (int16_t)atoi(argv[1]);
    char* ip = argv[2];

    int16_t dataPort = (int16_t)atoi(argv[3]);

    char* goose_interface = NULL;

    bool automaticOperationMode = (bool)atoi(argv[4]);

    if (argc == 6)
        goose_interface = argv[5];

    /* Create Circuit Breaker */

    printf("Creating Circuit Breaker...\n\n");
    circuitBreaker = new CircuitBreaker(id, ip, dataPort, goose_interface, automaticOperationMode);

    /* SetUp Circuit Breaker */

    printf("Setting up Circuit Breaker...\n\n");
    circuitBreaker->setUp();

    printf("Running Circuit Breaker...\n\n");
    circuitBreaker->run();

    return 0;
}