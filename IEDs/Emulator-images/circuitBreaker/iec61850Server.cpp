#include "iec61850Server.h"

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

/**
 * @brief Handles control actions for binary output signals in the IEC 61850 server.
 *
 * @param action The control action to be performed.
 * @param parameter A pointer to the control handler parameters, which includes the circuit breaker model and control signal.
 * @param value The new value for the binary output signal.
 * @param test A boolean flag indicating whether the action is a test action.
 * @return ControlHandlerResult The result of the control action handling.
 */
static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value, bool test)
{

    CircuitBreakerModel *model = (CircuitBreakerModel*)parameter;

    if (test)
        return CONTROL_RESULT_OK;

    bool newState = MmsValue_getBoolean(value);

   model->setOperatorState(newState);

    return CONTROL_RESULT_OK;
}

/**
 * @brief Perform check handler for control actions.
 * 
 * @param action The control action to be performed.
 * @param parameter User-defined parameter.
 * @param ctlVal The control value.
 * @param test A boolean flag indicating whether the action is a test action.
 * @param interlockCheck A boolean flag indicating whether interlock checks are required.
 * @param connection The client connection attempting the control action.
 * @return CheckHandlerResult The result of the check, indicating whether the control action is accepted or temporarily unavailable.
 */
static CheckHandlerResult
performCheckHandler(ControlAction action, void* parameter, MmsValue* ctlVal, bool test, bool interlockCheck, ClientConnection connection)
{
    IEC61850Server* server = (IEC61850Server*)parameter;
    
    if (server->controllingClient == NULL) {
        printf("Client takes control -> switch to remote control operation mode\n");
        server->controllingClient = connection;
        server->automaticOperationMode = false;
    }

    /* test command not accepted if mode is "on" */
    if (test)
        return CONTROL_TEMPORARILY_UNAVAILABLE;

    /* If there is already another client that controls the device reject the control attempt */
    if (server->controllingClient == connection)
        return CONTROL_ACCEPTED;
    else
        return CONTROL_TEMPORARILY_UNAVAILABLE;
}

static void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    CircuitBreakerModel* model = (CircuitBreakerModel*)parameter;

    if (GooseSubscriber_isValid(subscriber))
    {
        const MmsValue* dataset = GooseSubscriber_getDataSetValues(subscriber);
        const MmsValue* value = MmsValue_getElement(dataset, 0);
        bool newState = MmsValue_getBoolean(value);
        model->setSafeState(newState);
    }
}

/***********************************************
 *                                             *
 *          IEC 61850 Server Functions         *
 *                                             *
 ***********************************************/

IEC61850Server::IEC61850Server(int port, char* gooseInterface, CircuitBreakerModel* model)
{
    printf("Starting IEC61850SERVER\n");

    this->model = model;
    this->port = port;

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

    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_ALL, ACCESS_POLICY_ALLOW);

    //set_control_handlers();

    IedServer_setPerformCheckHandler(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator,
        (ControlPerformCheckHandler) performCheckHandler, (void*)this);

    IedServer_setControlHandler(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator, (ControlHandler) controlHandlerForBinaryOutput,
                (void*)model);

    /* Initialize process values */

    MmsValue* Operator_stVal = IedServer_getAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator_stVal);
    MmsValue_setBitStringFromInteger(Operator_stVal, 0); /* set Operator Break to OFF */

    gooseReceiver = GooseReceiver_create();

    GooseReceiver_setInterfaceId(gooseReceiver, DEF_GOOSE_INTERFACE);

    char path[100];
    snprintf(path, sizeof(path), "PM%02dLogicalDevice/LLN0$GO$securityEvent", model->id);

    GooseSubscriber subscriber = GooseSubscriber_create(path, NULL);

    uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00, model->id};
    GooseSubscriber_setDstMac(subscriber, dstMac);
    GooseSubscriber_setAppId(subscriber, model->id);

    GooseSubscriber_setListener(subscriber, gooseListener, (void*)model);

    GooseReceiver_addSubscriber(gooseReceiver, subscriber);
}

bool
IEC61850Server::startServer()
{
    /* Start the MMS server */
    IedServer_start(iedServer, port);

    /* Start the GOOSE receiver */
    GooseReceiver_start(gooseReceiver);

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
        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        IedServer_lockDataModel(iedServer);
        pthread_mutex_lock(model->data.dataLock);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_CircuitBreaker_t, &iecTimestamp);
        IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_CircuitBreaker_stVal, *model->data.currentState);
        IedServer_updateQuality(iedServer, IEDMODEL_LogicalDevice_GGIO1_CircuitBreaker_q, QUALITY_VALIDITY_GOOD | QUALITY_SOURCE_SUBSTITUTED);

        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_Security_t, timestamp);
        IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_Security_stVal, model->data.safeState);
        IedServer_updateQuality(iedServer, IEDMODEL_LogicalDevice_GGIO1_Security_q, QUALITY_VALIDITY_GOOD | QUALITY_SOURCE_SUBSTITUTED);

        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator_t, timestamp);
        IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator_stVal, model->data.operatorState);
        IedServer_updateQuality(iedServer, IEDMODEL_LogicalDevice_GGIO1_Operator_q, QUALITY_VALIDITY_GOOD | QUALITY_SOURCE_SUBSTITUTED);

        pthread_mutex_unlock(model->data.dataLock);
        IedServer_unlockDataModel(iedServer);

        sleep_mss(100);
    }
}

void
IEC61850Server::stopServer()
{
    IedServer_stop(iedServer);
    GooseReceiver_stop(gooseReceiver);
    
}

IEC61850Server::~IEC61850Server()
{
    if (iedServer != nullptr)
    {
        IedServer_stop(iedServer);
        IedServer_destroy(iedServer);
    }
    iedServer = nullptr;

    if (gooseReceiver != nullptr)
    {
        GooseReceiver_stop(gooseReceiver);
        GooseReceiver_destroy(gooseReceiver);
    }
    gooseReceiver = nullptr;
}