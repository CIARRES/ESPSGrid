# Docker Images

To simplify **deployment, scalability, and reproducibility**, the **IEDs, RTU, and SCADA** components have been containerized using **Docker**. This approach allows for easy distribution across different hardware architectures and ensures a consistent runtime environment, minimizing configuration issues.  

By using **pre-built Docker images**, users can quickly deploy the system without needing to manually configure dependencies. Additionally, for those who wish to modify or extend the functionality of the components, we provide instructions for **building custom images**

## Pull from Docker Hub

Build images are available in docker hub to be pulled.

- [Smart Power Meter](https://hub.docker.com/repository/docker/javiersande/sg-powermeter)
- [Circuit Breaker](https://hub.docker.com/repository/docker/javiersande/sg-circuitbreaker)
- [RTU](https://hub.docker.com/repository/docker/javiersande/sg-rtu)
- [SCADA](https://hub.docker.com/repository/docker/javiersande/sg-scada)

Available architectures are:

- `linux/arm64`  
- `linux/arm/v6`  
- `linux/arm/v7`  
- `linux/amd64`  

## Custom build

In the case you need to deploy the images on other architectures or you want to make modifications to the source code of the IEDs, you would need to build the docker images:

```
cd IEDs/Emulator-images/
docker build -f ./circuitBreaker/Dockerfile -t circuitbreaker:latest .
docker build -f ./smartMeter/Dockerfile -t smartmeter:latest .
```
Next, 

```
cd RTU/OpenPLC_v3_iec61850/
docker build -t sg-rtu:latest .
```
Finally, 

```
cd Scada/ScadaBR-Container/
docker build -t sg-scada:latest .
```
