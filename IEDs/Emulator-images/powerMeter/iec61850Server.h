#include "common.h"

#include "iec61850_server.h"
#include "ladder.h"

class IEC61850Server {
public:
    /**
     * @brief Construct a new IEC61850Server object
     * 
     * @param port  The port number.
     * @param gooseInterface The GOOSE interface identifier.
     * @param model The power meter data model.
     */
    IEC61850Server(int port, char* gooseInterface, PowerMeterModel* model);

    /**
     * @brief Destroy the IEC61850Server object
     * 
     */
    ~IEC61850Server();

    /**
     * @brief Start the IEC61850 server at the specified port
     * 
     * @return true Server started successfully.
     * @return false Server failed to start.
     */
    bool startServer();

    /**
     * @brief Close the MMS server and release resources.
     */
    void stopServer();

    /**
     * @brief Start exchange of IEC61850 data.
     * 
     * @param model The power meter model.
     * @return void* 
     */
    void startDataExchange();

    /**
     * @brief Update the server with the latest data from the power meter model.
     * 
     * @param timestamp The current timestamp.
     * @param model The power meter model.
     */
    void updateServer();

private:

    IedServer iedServer;

    PowerMeterModel* model;

    int port;

    char gooseInterface[INTERFACE_SIZE + 1];
};
