#pragma once

#include <stdlib.h>
#include <pthread.h>
#include <cstdint>

#define DEF_GOOSE_INTERFACE "eth0"
#define MMS_PORT 102

#define IP_SIZE 16
#define INTERFACE_SIZE 100

#define THREE_PHASES 3

#define LATENCY 2000

/**
 * @brief Structure to hold the data for a power meter.
 * 
 * This structure contains the state and measurement data for a power meter,
 * including current and voltage measurements, and a mutex for thread-safe access.
 */
struct smData {
    bool safeState;              /**< Indicates if the power meter is in a safe state. */
    double* currents;            /**< Pointer to an array of current measurements. */
    double* voltages;            /**< Pointer to an array of voltage measurements. */
    pthread_mutex_t* dataLock;   /**< Mutex for protecting access to the data. */
} typedef PowerMeterData;

/**
 * @brief Class to represent a power meter model.
 * 
 * This class contains the configuration and state information for a power meter,
 * including network settings, measurement data, and safety thresholds.
 */

class PowerMeterModel {

public:

 bool running;                /**< Indicates if the power meter is running. */
    int16_t id;               /**< The power meter identifier. */
    PowerMeterData data;      /**< Measurement data and state information. */
    int16_t safetyThreshold;  /**< Safety threshold value for the power meter. */

    /**
     * @brief Construct a new Power Meter Model object
     * 
     * @param id The power meterModel identifier.
     */
    PowerMeterModel(int16_t id, int16_t safetyThreshold) : id(id), safetyThreshold(safetyThreshold)
    {
        data.dataLock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        data.safeState = true;
        data.currents = (double*)malloc(THREE_PHASES * sizeof(double));
        data.voltages = (double*)malloc(THREE_PHASES * sizeof(double));
    }
};

extern PowerMeterModel* model;