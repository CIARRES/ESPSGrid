# Smart Grid Co-Simulation Framework

This environment enables us to simulate both the regular operation of the smart grid, and different type of cyber-attacks that exploit cyber-layer devices and protocols, allowing us to observe their physical consequences.

## Physical layer - Simulink

The network chosen for our simulation is the IEEE- 14 test grid, a standard test case in power systems re- search and education featuring 14 nodes connected by 20 branches.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/Grid.png" width="600">
</p>

To simulate this model, follow these steps:

1. Open the folder [Physical-Model/IEEE14 Smart Grid](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/Physical-Model/IEEE14%20Smart%20Grid) Smart Grid in MATLAB.

2. Next, open and run the file [slblocks.m](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14%20Smart%20Grid/slblocks.m) to load some custom blocks in our the model.

3. Finally, open the file [Ieee14_CC.slx](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14%20Smart%20Grid/Ieee14_CC.slx) with Simulink.

### Power Meter and Circuit Breaker

Given the challenges and limitations of integrating new functionalities and communication protocols directly into Simulink, our approach divides the emulation of IEDs into two essential components. The physical com- ponent, responsible for tasks such as reading line values (in the case of a power meter) or controlling the electricity flow (as with a circuit breaker), is implemented as a [MATLAB/Simulink subsystem](https://es.mathworks.com/help/simulink/subsystems.html).

As shown, we maintain the physical inputs and outputs corresponding to the current lines, numbered 1 to 6 and identified by the letters ABC. Our addition includes the integration of UDP ports that are used to enable the transmission of measured data from the power meter to the externally executed C++ program that emulates the IED.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/PM.png" height="250">
</p>
<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/CB.png" height="180">
</p>


In our case, we have followed these rules to establish the UDP ports for each IED:
* For the power meter, we have a UDP module sending measurements on port 1XXXX, where XXXX corresponds to the device ID. For example, power meter 1 will be listening on port 20001.

* For the circuit breaker, there is a UDP listener on port 2XXXX, where XXXX corresponds to the device ID. For example, CB 1 will be listening on port 20001.

### Using other physical models

This framework enables co-simulations with other physical models in MATLAB/Simulink. To achieve this, you need to replace the meters and circuit breakers in the target model with the subsystems available in [IEDs/Simulink](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/IEDs/Simulink).

Once all the power meters and circuit breakers are in place, you must configure the UDP ports used to send and receive data to and from the external program responsible for emulating the logical and communication aspects of the corresponding IED.

## Cyber layer - Emulation

In the realm of communication protocols, various layers are defined. Starting from the bottom, the first two layers implement protocols GOOSE and MMS, integral to the IEC 61850 standard. This standard specifically addresses communication within substation systems, focusing on IEDs. At the topmost level, the Modbus protocol supports data exchange between the RTU and the SCADA system. The RTU sends collected data from IEDs to the SCADA system for visualization by operators.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/Communications.png" width="400">
</p>

### Power Meter and Circuit Breaker

For each IED, the logical component, managing communication and logical tasks, is developed in a C++ program. Despite the separate implementation of the physical layer (simulated using Simulink) within our framework, both components conceptually represent features of the same physical device.

Given that for a realistic simulation, each IED needs to function as an independent device with its own IP and MAC address within its respective network, virtualization becomes essential. However, due to the number of IEDs to be emulated and the simplicity of the programs they run, using virtual machines is not scalable. Therefore, the programs responsible for emulating IED logic are encapsulated into Docker images for execution within containers. This approach allows us to deploy a network of IEDs quickly, efficiently, and at scale using [Docker Compose](https://docs.docker.com/compose/). Moreover, by utilizing Docker’s [IPVlan network driver](https://docs.docker.com/network/drivers/ipvlan/) we can integrate these containers into an external network, enabling connection to other systems such as the VM running the RTU or the hardware used for Simulink modeling.

Prior to the execution of the IEDs we need to build the docker images:

```
cd IEDs/Emulator-images/
docker build -f ./circuitBreaker/Dockerfile -t circuitbreaker:latest .
docker build -f ./powerMeter/Dockerfile -t powermeter:latest .
```

To run our containers, we have two options.

First option is to run an IED independently:

1. Create the IPvlan network:
   ```
   docker network create -d ipvlan --subnet=10.0.0.0/16 --gateway=10.0.0.1 -o ipvlan_mode=l2 -o parent=eth0 transportNetwork
   ```
2. Run the IED container.
  * For th power meter:
     ```
      docker run -id --name powermeter<id> \
        --network transportNetwork --ip <ip> \
        -e PM_ID=<id> \
        -e PM_IP=<ip> \
        -e PM_GATEWAY=10.0.0.1 \
        -e S_IP=<Simulink ip> \
        -e S_DATA_PORT=<Data port> \
        -e SAFE_THRESHOLD=5000 \
        powermeter:latest
     ```

  * For the circuit breaker:
      ```
      docker run -d --name circuitbreaker<id> \
      --network transportNetwork --ip <ip> \
      -e CB_ID=<id> \
      -e CB_IP=<ip> \
      -e CB_GATEWAY=10.0.0.1 \
      -e S_IP=<Simulink ip> \
      -e S_PORT=<port> \
      circuitbreaker:latest
      ```

The second option is to run all the IEDs together using Docker Compose. To do this, from the root directory of the project, you need to execute:

```
cd Comunicactions-network
docker compose up
```

### RTU - OpenPLC 61850

In our model, the RTU serves as the intermediary between the IEDs and the SCADA system, responsible for collecting data from each IED and transmitting it to SCADA. It also receives commands from SCADA operators and forwards them to the IEDs operating as circuit breakers. To achieve this, we used the [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850) tool, developed from Thiago Alves’s [OpenPLC project](https://github.com/thiagoralves/OpenPLC_v3).

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/OpenPLC.png" width="600">
</p>

For the [installation](https://github.com/smartgridadsc/OpenPLC61850?tab=readme-ov-file#4-installation) and [configuration](https://github.com/smartgridadsc/OpenPLC61850?tab=readme-ov-file#51-configuration) of the tool, we recommend following the instructions in the [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850). During the configuration, you will need the following files:

* [IEC 61131-3 (ST) PLC program](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/OpenPLC-program/program.st)
* [IED SCL files](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/IEDs/OpenPLC-ICL-files)
* Server SCL file: Since this functionality of OpenPLC is not used but the file is mandatory, you can leave the example file.

Once OpenPLC61850 is configured, you can start it by pressing the Start PLC button on the web interface.

### SCADA - SCADA BR

In order to connect OpenPLC to SCADA BR and create an HMI 

We also recommend to watch Thiago Alves' [tutorial](https://youtu.be/KrcL6lhAHKw?feature=shared) to get a better understanding on how Scada BR is connected to OpenPLC.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/SCADA.png" width="600">
</p>

## Recommended setup

In our experiments, simulations were executed using a computer running Simulink and virtual machines hosting a [pfSense](https://www.pfsense.org/download/) router, [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850), and [SCADA BR](https://github.com/ScadaBR). The execution of containers, running different IEDs, was divided between two Raspberry Pi connected to the computer via a switch.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/docs/images/HW.png" width="400">
</p>

## References

For the creation of this project, the following open-source projects have been employed:

* [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850)
* [OpenPLC](https://autonomylogic.com)
* [SCADA BR](https://github.com/ScadaBR)
* [libiec61850](https://libiec61850.com)
