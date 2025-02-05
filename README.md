# ESPSGrid: Emulation-Simulation for Power Smart

ESPSGrid framework provides a environment to simulate both the standard operation of a smart grid and various types of cyber-attacks targeting cyber-layer devices and protocols. It enables the observation of their physical consequences, offering valuable insights into grid resilience and vulnerabilities.

1. [Implementation Details](#implementation-details)
2. [Launch the Default Model](#launch-the-default-model)
3. [Using Your Own Model](#using-your-own-model)
4. [References](#references)

## Implementation Details

This section outlines the various layers and devices that constitute the ESPSGrid Framework.

### Physical layer - Simulink

The network chosen for our simulation is the IEEE-14 test grid, a standard test case in power systems research and education featuring 14 nodes connected by 20 branches.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/Grid.png" width="600">
</p>

#### Power Meter and Circuit Breaker

Given the challenges and limitations of integrating new functionalities and communication protocols directly into Simulink, our approach divides the emulation of IEDs into two essential components. The physical component, responsible for tasks such as reading line values (in the case of a power meter) or controlling the electricity flow (as with a circuit breaker), is implemented as a [MATLAB/Simulink subsystem](https://es.mathworks.com/help/simulink/subsystems.html).

As shown, we maintain the physical inputs and outputs corresponding to the current lines, numbered 1 to 6 and identified by the letters ABC. Our addition includes the integration of UDP ports that are used to enable the transmission of measured data from the power meter to the externally executed C++ program that emulates the IED.

<div style="display: flex; justify-content: center; align-items: center;">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/PM.png" height="200">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/CB.png" height="200">
</div>


In our case, we have followed these rules to establish the UDP ports for each IED:

* For the power meter, we have a UDP module sending measurements on port 1XXXX, where XXXX corresponds to the device ID. For example, power meter 1 will be sending to port 10001.

* For the circuit breaker, there is a UDP listener on port 2XXXX, where XXXX corresponds to the device ID. For example, CB 1 will be listening on port 20001.

### Cyber layer - Emulation

In the realm of communication protocols, various layers are defined. Starting from the bottom, the first two layers implement protocols GOOSE and MMS, integral to the IEC 61850 standard. This standard specifically addresses communication within substation systems, focusing on IEDs. At the topmost level, the Modbus protocol supports data exchange between the RTU and the SCADA system. The RTU sends collected data from IEDs to the SCADA system for visualization by operators.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/Communications.png" width="400">
</p>

#### Power Meter and Circuit Breaker

For each IED, the logical component, managing communication and logical tasks, is developed in a C++ program. Despite the separate implementation of the physical layer (simulated using Simulink) within our framework, both components conceptually represent features of the same physical device.

Given that for a realistic simulation, each IED needs to function as an independent device with its own IP and MAC address within its respective network, virtualization becomes essential. However, due to the number of IEDs to be emulated and the simplicity of the programs they run, using virtual machines is not scalable. Therefore, the programs responsible for emulating IED logic are encapsulated into Docker images for execution within containers. This approach allows us to deploy a network of IEDs quickly, efficiently, and at scale using [Docker Compose](https://docs.docker.com/compose/). Moreover, by utilizing Docker’s [IPVlan network driver](https://docs.docker.com/network/drivers/ipvlan/) we can integrate these containers into an external network, enabling connection to other systems such as the container running the RTU or the hardware used for Simulink modeling.

#### RTU - OpenPLC 61850

In our model, the RTU serves as the intermediary between the IEDs and the SCADA system, responsible for collecting data from each IED and transmitting it to SCADA. It also receives commands from SCADA operators and forwards them to the IEDs operating as circuit breakers. To achieve this, we used the [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850) tool, developed from Thiago Alves’s [OpenPLC project](https://github.com/thiagoralves/OpenPLC_v3).

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/OpenPLC.png" width="600">
</p>

#### SCADA - SCADA BR

The SCADA system is the top layer of our framework, providing visual monitoring of the grid for the operator and enabling operations through commands, such as open and close orders for the circuit breakers. To implement this, we use a [containerized version](https://github.com/CIARRES/ScadaBR-Container) of the original [SCADA BR](https://github.com/ScadaBR) project.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/SCADA.png" width="600">
</p>

## Launch the Default Model

### Requirements

* Linux system with Docker installed
* MATLAB/Simulink 2023b or higher

### Installation Steps

This section provides a step-by-step guide to install and launch the default example included with the tool.

1. **Clone the Repository**

   Begin by cloning the repository and its submodules:

   ```bash
   git clone https://github.com/CIARRES/SG-Co-Simulation-Framework.git
   cd SG-Co-Simulation-Framework
   git submodule update --init
   ```

2. **Pull Docker Images**

   Before running the IEDs, RTU and SCADA, you need to pull the Docker images:

   ```bash
   cd IEDs/Emulator-images/
   docker pull javiersande/sg-powermeter:latest
   docker pull javiersande/sg-circuitbreaker:latest
   docker pull javiersande/sg-rtu:latest
   docker pull javiersande/sg-scada:latest
   ```

3. **Set Up the Communication Infrastructure**

   The next step is to set up the communication infrastructure, which includes SCADA, RTU, and IEDs (power meters and circuit breakers). To simplify this process, we provide a [Docker Compose file](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Comunicactions-network/IEEE14/compose.yml).

   ```bash
   cd Comunicactions-network/IEEE14
   docker compose up
   ```

   This command creates a network consisting of four power meters, four circuit breakers, an RTU, and a SCADA.

   Note: We recommend waiting `2-3 minutes` before proceeding to the next step to allow the IEDs sufficient time to initialize and become ready for incoming connections.

4. **Configure RTU**

   The RTU is launched as part of the Docker Compose infrastructure. Once it is running, it needs to be configured to connect with the various IEDs. By default, the RTU image includes a custom program designed to interact with the provided infrastructure.

   * Log in to the OpenPLC web portal at `http://10.0.0.3:8080` using the default credentials: `openplc:openplc`.

   * Start the program by clicking the **Start PLC** button.

   Once the program is running, you can monitor the IEDs on the **Monitoring** page, accessible via the left-hand menu bar.

    <p align="center">
    <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/RTU_setup.png" width="400">
    </p>

5. **Configure SCADA System**

   Once the RTU is running, you can connect the SCADA system. To simplify this process, a [configuration file](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Scada/scadabr.json) is provided to automatically set up the data point connections with the RTU.

   * Log in to the SCADA web portal at `http://10.0.0.4:8080/ScadaBR` using the default credentials: `admin:admin`.
   * Navigate to the project import page.
   * Copy the contents of the [configuration file](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Scada/scadabr.json) into the import field and click the **Import** button.
   * Log out and log back in to the SCADA web server.

    <p align="center">
    <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/SCADA_setup.png" width="400">
    </p>

   You can now monitor the grid through the **Graphical View** interface.

    <p align="center">
    <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/SCADA_View.png" width="400">
    </p>

6. **Start Physical Layer Simulation**

   Open the IEEE14 [project](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14/) in your local MATLAB installation.

   Before launching the model, execute the [slblocks.m](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14/slblocks.m) file to load the [custom blocks](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14/source_mod.slx) required by the model.

   Finally, open and run the Simulink [model](https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/Physical-Model/IEEE14/Ieee14.slx).

    <p align="center">
    <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/Simulink.png" width="400">
    </p>

### Example setup

In our experiments, simulations were executed using a Window 10 computer running Simulink. The execution of containers, running different IEDs, RTU and Scada, was performed in a Raspberry Pi connected to the computer via a switch.

<p align="center">
  <img src="https://github.com/CIARRES/SG-Co-Simulation-Framework/blob/main/documentation/images/HW.png" width="400">
</p>


## Using Your Own Model

This framework supports co-simulations with other physical models. To use your own model, you will need to make adjustments across the various layers.

### Physical model

This framework enables co-simulations with other physical models in MATLAB/Simulink. To achieve this, you need to replace the meters and circuit breakers in the target model with the subsystems available in [IEDs/Simulink](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/IEDs/Simulink).

Once all the power meters and circuit breakers are in place, you must configure the UDP ports used to send and receive data to and from the external program responsible for emulating the logical and communication aspects of the corresponding IED.

### RTU

To connect your IEDs to the RTU, you need to modify the ST program loaded into the OpenPLC, as well as the SCL files that contain the information required to establish the connection between the RTU and the IEDs through MMS (IEC 61850).

You can use the [ST program](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/RTU/OpenPLC%20Program) and [SCL files](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/IEDs/OpenPLC-ICL-files) from our example as a reference. Additionally, we provide a [SCL file generator](https://github.com/CIARRES/SG-Co-Simulation-Framework/tree/main/IEDs/OpenPLC-ICL-files/generator) to generate SCL client files for new IEDs by simply providing the ID, IP address, and IP gateway of your new IED.

We also recommend following the instructions in the [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850) repository for further guidance.

### SCADA

To connect OpenPLC to SCADA BR and create an HMI, you need to configure OpenPLC as a data source. We recommend watching Thiago Alves' [tutorial](https://youtu.be/KrcL6lhAHKw?feature=shared) for detailed instructions on correctly connecting SCADA BR to OpenPLC.

## References

For the creation of this project, the following open-source projects have been employed:

* [OpenPLC61850](https://github.com/smartgridadsc/OpenPLC61850)
* [OpenPLC](https://autonomylogic.com)
* [SCADA BR](https://github.com/ScadaBR)
* [libiec61850](https://libiec61850.com)
