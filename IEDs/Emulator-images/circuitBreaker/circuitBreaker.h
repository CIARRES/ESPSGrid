#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <cstdint>


#include "common.h"
#include "iec61850Server.h"
#include "simlink.h"
#include "hal_thread.h"

class CircuitBreaker {
public:

    /**
     * @brief Construct a new Circuit Breaker object
     * 
     * @param id The circuit breaker identifier.
     * @param ip The circuit breaker IP address.
     * @param dataPort The circuit breaker data port.
     * @param goose_interface The circuit breaker GOOSE interface.
     * @return CircuitBreaker The circuit breaker object.
     */
    CircuitBreaker(int16_t id, char* ip, int16_t dataPort, char* goose_interface);

    /**
     * @brief Destroy the Smart Meter object
     * 
     */
    ~CircuitBreaker();

    /**
     * @brief Set up the circuit breaker Simulink connection and MMS server.
     * 
     * @return void
     */
    void setUp();

    /**
     * @brief Start the circuit breaker.
     * 
     */
    void run();

    /**
     * @brief Display the circuit breaker data.
     * 
     * @return void
     */
    void displayData();

    /**
     * @brief Stop the circuit breaker.
     * 
     * @return void
     */
    void stop();

private:
    CircuitBreakerModel* model;
    IEC61850Server* server;
    SimLink* simLink;

    int16_t id;                  /** Unique identifier for the circuit breaker. */
    char* ip;                    /** IP address of the circuit breaker. */
    char* gooseInterface;       /** Network interface for GOOSE communication. */
    int16_t dataPort;            /** Port number for data communication. */

    /**
     * @brief Create a Simulink model for the circuit breaker.
     * 
     * @param model The circuit breaker model.
     * @return SimLinkModel* The created Simulink model.
     */
    SimLinkModel* createSimlinkModel();
};