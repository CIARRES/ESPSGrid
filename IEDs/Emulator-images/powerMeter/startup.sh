#!/bin/bash

if [ -z "$PM_ID" ]; then
    echo "Error: PM_ID environment variable is not set."
    exit 1
fi

if [ -z "$PM_IP" ]; then
    echo "Error: PM_IP environment variable is not set."
    exit 1
fi

if [ -z "$PM_GATEWAY" ]; then
    echo "Error: PM_GATEWAY environment variable is not set."
    exit 1
fi

if [ -z "$S_IP" ]; then
    echo "Error: S_IP environment variable is not set."
    exit 1
fi

if [ -z "$S_DATA_PORT" ]; then
    echo "Error: S_DATA_PORT environment variable is not set."
    exit 1
fi

if [ -z "$SAFE_THRESHOLD" ]; then
    echo "Error: SAFE_THRESHOLD environment variable is not set."
    exit 1
fi

echo "Generating model for PM $PM_ID..."
./modelGenerator $PM_ID $PM_IP $PM_GATEWAY && \
make model &>/dev/null && \

echo "Compiling PM $PM_ID..." && \
make &>/dev/null && \
./powerMeter $PM_ID $S_IP $S_DATA_PORT $SAFE_THRESHOLD eth0