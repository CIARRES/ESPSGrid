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
 * @brief The structure to hold the data for a circuit breaker.
 * 
 */
struct cbData {
    bool safeState;              /**< Indicates if the power line is in a safe state. */
    bool operatorState;          /**< Indicates the state of the operator. */
    bool* currentState;          /**< Indicates the current state of the circuit breaker. */
    pthread_mutex_t* dataLock;   /**< Mutex for protecting access to the data. */
} typedef CircuitBreakerData;

/**
 * @brief The class to represent a circuit breaker model.
 * 
 * This class contains the configuration and state information for a circuit breaker,
 * including network settings and state information.
 */
class CircuitBreakerModel {

public:

    int16_t id;                  /**< The circuit breaker identifier. */
    bool running;                /**< Indicates if the smart meter is running. */
    CircuitBreakerData data;     /**< Status data and state information. */

    /**
     * @brief Construct a new Circuit Breaker Model object
     * 
     * @param id The circuit breaker identifier.
     */
    CircuitBreakerModel(int16_t id) : id(id)
    {
        data.dataLock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        data.safeState = true;
        data.operatorState = false;
        data.currentState = NULL;
    }

    /**
     * @brief Set the operator state.
     * 
     * @param state The new operator state.
     */
    void setSafeState(bool state)
    {
        pthread_mutex_lock(data.dataLock);
        data.safeState = state;
        updateCicuitBreakerDataStatus();
        pthread_mutex_unlock(data.dataLock);
    }

    /**
     * @brief Set the operator state.
     * 
     * @param state The new operator state.
     */
    void setOperatorState(bool state)
    {
        pthread_mutex_lock(data.dataLock);
        data.operatorState = state;
        updateCicuitBreakerDataStatus();
        pthread_mutex_unlock(data.dataLock);
    }

private:

    /**
     * @brief Update the circuit breaker data status.
     * 
     */
    void updateCicuitBreakerDataStatus()
    {
        *data.currentState = data.safeState && data.operatorState;
    }
};