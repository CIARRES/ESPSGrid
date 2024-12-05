#include "iec61850Server.h"

#include "iec61850_server.h"
#include "ladder.h"
#include "static_model.h"


/***********************************************
 *                                             *
 *          Utility Functions                  *
 *                                             *
 ***********************************************/

/**
 * @brief Sleep for a specified number of milliseconds.
 * 
 * @param milliseconds The number of milliseconds to sleep.
 */
void
sleep_mss(int milliseconds)
{
    struct timespec ts = {milliseconds / 1000, (milliseconds % 1000) * 1000000};
    nanosleep(&ts, NULL);
}

/**
 * @brief Wrapper function for exchanging IEC 61850 data.
 * 
 * @param args server instance.
 * @return void* 
 */
void
*exchangeIEC61850Data(void *args)
{
    printf("Starting IEC61850 data exchange\n");
    IEC61850Server* server = (IEC61850Server*)args;
    printf("Server: %p\n", server);
    server->updateServer();

    return NULL;
}


/***********************************************
 *                                             *
 *          Handler Functions                  *
 *                                             *
 ***********************************************/

/**
 * @brief Event handler for Report Control Block (RCB) events.
 * 
 * @param parameter User-defined parameter.
 * @param rcb The Report Control Block.
 * @param connection The client connection.
 * @param event The RCB event type.
 * @param parameterName The name of the parameter.
 * @param serviceError The service error code.
 */
static void rcbEventHandler(void* parameter, ReportControlBlock* rcb, ClientConnection connection, IedServer_RCBEventType event, const char* parameterName, MmsDataAccessError serviceError)
{
    //printf("RCB: %s event: %i\n", ReportControlBlock_getName(rcb), event);

    if ((event == RCB_EVENT_SET_PARAMETER) || (event == RCB_EVENT_GET_PARAMETER)) {
        printf("  param:  %s\n", parameterName);
        printf("  result: %i\n", serviceError);
    }

    if (event == RCB_EVENT_ENABLE) {
        char* rptId = ReportControlBlock_getRptID(rcb);
        printf("   rptID:  %s\n", rptId);
        char* dataSet = ReportControlBlock_getDataSet(rcb);
        printf("   datSet: %s\n", dataSet);

        free(rptId);
        free(dataSet);
    }
}

/**
 * @brief Connection handler for client connections.
 * 
 * @param self The IEC 61850 server instance.
 * @param connection The client connection.
 * @param connected Connection status.
 * @param parameter User-defined parameter.
 */
static void connectionHandler(IedServer self, ClientConnection connection, bool connected, void* parameter)
{
    if (connected)
        printf("Connection opened\n");
    else
        printf("Connection closed\n");
}

/**
 * @brief Control handler for control actions.
 * 
 * @param action The control action.
 * @param parameter User-defined parameter.
 * @param value The MMS value.
 * @param test Test flag.
 * @return ControlHandlerResult The result of the control action.
 */
static ControlHandlerResult controlHandler(ControlAction action, void* parameter, MmsValue* value, bool test)
{
    if (test)
        return CONTROL_RESULT_FAILED;

    std::string &node_string = *(static_cast<std::string*>(parameter));

    printf("ControlHandler called for %s\n", node_string.c_str());
    return CONTROL_RESULT_OK;
}

/**
 * @brief Event handler for GOOSE Control Block (GoCB) events.
 * 
 * @param goCb The GOOSE Control Block.
 * @param event The event type.
 * @param parameter User-defined parameter.
 */
static void goCbEventHandler(MmsGooseControlBlock goCb, int event, void* parameter)
{
    printf("Access to GoCB: %s\n", MmsGooseControlBlock_getName(goCb));
    printf("         GoEna: %i\n", MmsGooseControlBlock_getGoEna(goCb));
}


/***********************************************
 *                                             *
 *          IEC 61850 Server Functions         *
 *                                             *
 ***********************************************/

IEC61850Server::IEC61850Server(int port, char* gooseInterface, PowerMeterModel* model)
{

    this->port = port;
    strncpy(this->gooseInterface, gooseInterface, INTERFACE_SIZE);
    this->model = model;

    printf("Starting IEC61850SERVER\n");

    /* Create new server configuration object */
    IedServerConfig config = IedServerConfig_create();

    /* Set buffer size for buffered report control blocks to 200000 bytes */
    IedServerConfig_setReportBufferSize(config, 200000);

    /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
    IedServerConfig_setEdition(config, IEC_61850_EDITION_2);

    /* Set the base path for the MMS file services */
    IedServerConfig_setFileServiceBasePath(config, "./vmd-filestore/");

    /* disable MMS file service */
    IedServerConfig_enableFileService(config, false);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, false);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 2);

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    IedServer_setRCBEventHandler(iedServer, rcbEventHandler, NULL);

    //IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_ALL, ACCESS_POLICY_ALLOW);

    /* set GOOSE interface for all GOOSE publishers (GCBs) */
    IedServer_setGooseInterfaceId(iedServer, gooseInterface);
    IedServer_setGooseInterfaceIdEx(iedServer, IEDMODEL_LogicalDevice_LLN0, "securityEvent", gooseInterface);

    IedServer_setGoCBHandler(iedServer, goCbEventHandler, NULL);
}

bool
IEC61850Server::startServer()
{
    /* Start the MMS server */
    IedServer_start(iedServer, port);

    /* Start GOOSE publishing */
    IedServer_enableGoosePublishing(iedServer);

    if (!IedServer_isRunning(iedServer)) {
        printf("Error: Could not setup MMS server\n");
        IedServer_destroy(iedServer);
        return false;
    }

    return true;
}

void
IEC61850Server::startDataExchange()
{
    pthread_t receivingThread;
    pthread_create(&receivingThread, NULL, exchangeIEC61850Data, (void*)this);
}

void
IEC61850Server::updateServer() {

    struct timespec timer_start;
    clock_gettime(CLOCK_MONOTONIC, &timer_start);

    while(model->running)
    {
        uint64_t timestamp = Hal_getTimeInMs();

        pthread_mutex_lock(model->data.dataLock);
        int32_t vA = static_cast<int32_t>(model->data.voltages[0]);
        int32_t vB = static_cast<int32_t>(model->data.voltages[1]);
        int32_t vC = static_cast<int32_t>(model->data.voltages[2]);
        int32_t iA = static_cast<int32_t>(model->data.currents[0]);
        int32_t iB = static_cast<int32_t>(model->data.currents[1]);
        int32_t iC = static_cast<int32_t>(model->data.currents[2]);
        pthread_mutex_unlock(model->data.dataLock);

        Timestamp iecTimestamp;
        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        IedServer_lockDataModel(iedServer);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVA_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVA_mag_f, vA);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVB_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVB_mag_f, vB);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVC_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnVC_mag_f, vC);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIA_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIA_mag_f, iA);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIB_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIB_mag_f, iB);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIC_t, &iecTimestamp);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_AnIC_mag_f, iC);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_SecurityST_t, &iecTimestamp);
        IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_SecurityST_stVal, model->data.safeState);

        IedServer_unlockDataModel(iedServer);

        sleep_mss(100);
    }
}

void
IEC61850Server::stopServer()
{
    IedServer_stop(iedServer);
}

IEC61850Server::~IEC61850Server()
{
    if (iedServer != nullptr)
    {
        IedServer_stop(iedServer);
        IedServer_destroy(iedServer);
    }
    iedServer = nullptr;
}