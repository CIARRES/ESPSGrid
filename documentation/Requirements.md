# Requirements  

## Physical Layer Simulation  

The proposed physical model has been built using **MATLAB/Simulink 2023b**. Therefore, this version or a newer one is required to reproduce our example. However, **ESPSGrid is compatible with any Simulink model**, as long as the **UDP Client and Server** blocks are available.  

**Note:** During our experiments, we encountered connectivity issues in the **UDP communication** between Simulink and IED containers on **macOS** versions of MATLAB. We recommend using **Windows** or **Linux** platforms for better stability.  

## IED Emulation  

IEDs are emulated as **Docker containers**, deployed using **Docker Compose**. This deployment relies on an [IPvlan L2](https://docs.docker.com/engine/network/drivers/ipvlan/) network, which is **only available on Linux distributions**.  

We provide pre-built Docker images for the following architectures:  
- `linux/arm64`  
- `linux/arm/v6`  
- `linux/arm/v7`  
- `linux/amd64`  

If needed, images can be built for additional architectures, as explained in the [Docker Images section](https://github.com/CIARRES/ESPSGrid/tree/main/documentation/Images.md).  

**Note:** The SCADA image has been observed to fail on some **linux/arm/v7** devices due to **Java 1.8** compatibility issues.  
