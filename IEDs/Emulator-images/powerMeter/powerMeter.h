#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <cstdint>


#include "common.h"
#include "iec61850Server.h"
#include "simlink.h"
#include "hal_thread.h"

class PowerMeter {
public:

    /**
     * @brief Construct a new Power Meter object
     * 
     * @param id The power meter identifier.
     * @param ip The power meter IP address.
     * @param dataPort The power meter data port.
     * @param goose_interface The power meter GOOSE interface.
     * @param safetyThreshold The power meter safety threshold.
     * @return PowerMeter The power meter object.
     */
    PowerMeter(int16_t id, char* ip, int16_t dataPort, char* goose_interface, int16_t safetyThreshold);

    /**
     * @brief Destroy the Power Meter object
     * 
     */
    ~PowerMeter();

    /**
     * @brief Set up the power meter Simulink connection and MMS server.
     * 
     * @return void
     */
    void setUp();

    /**
     * @brief Start the power meter.
     * 
     */
    void run();

    /**
     * @brief Security monitoring function.
     * 
     */
    void securityMonitoring();

    /**
     * @brief Display the power meter data.
     * 
     * @return void
     */
    void displayData();

    /**
     * @brief Stop the power meter.
     * 
     * @return void
     */
    void stop();

private:
    PowerMeterModel* model;
    IEC61850Server* server;
    SimLink* simLink;

    int16_t id;                  /** Unique identifier for the power meter. */
    char* ip;                    /** IP address of the power meter. */
    char* gooseInterface;       /** Network interface for GOOSE communication. */
    int16_t dataPort;            /** Port number for data communication. */

    /**
     * @brief Create a Simulink model for the power meter.
     * 
     * @param model The power meter model.
     * @return SimLinkModel* The created Simulink model.
     */
    SimLinkModel* createSimlinkModel();
};