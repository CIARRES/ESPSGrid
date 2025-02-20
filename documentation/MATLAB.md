# MATLAB/Simulink Configuration  

To simulate **physical models** in MATLAB/Simulink, you need to **replace the meters and circuit breakers** in your target model with the **subsystems available in** [IEDs/Simulink](https://github.com/CIARRES/ESPSGrid/tree/main/IEDs/Simulink).  

Once all **power meters** and **circuit breakers** are in place, you must configure the **UDP ports** to properly send and receive data between **Simulink** and the **IED containerized instances**.  

## Smart Power Meter Configuration  

For the **Smart Power Meter**, the **IP address and port** of the **corresponding container** must be set as the **remote address and port** in Simulink.  

<p align="center">
  <img src="https://github.com/CIARRES/ESPSGrid/blob/main/documentation/images/PM%20setup.png" width="600">
</p>  

UDP Configuration for Smart Power Meter:  
- The **power meter container listens** on port **1XXXX**, where `XXXX` corresponds to the **device ID**.  
- For example, **Power Meter 1** listens on **port `10001`**.  
- Thus, in MATLAB, the **local port** must be set to **10001**.


## Circuit Breaker Configuration  

For the **Circuit Breaker**, the **local address** can be set to **`0.0.0.0`** (any interface) or to the **specific interface connected to the IED network**. The **local and remote ports** must match the **corresponding circuit breaker container**, and the **container's IP** must be set as the **remote address**.  

<p align="center">
  <img src="https://github.com/CIARRES/ESPSGrid/blob/main/documentation/images/CB%20setup.png" width="600">
</p>  

UDP Configuration for Circuit Breaker:  
- The **circuit breaker container sends data** through port **2XXXX**, where `XXXX` corresponds to the **device ID**.  
- For example, **Circuit Breaker 1** sends data on **port `20001`**.  
- In MATLAB, **both the remote and local ports** must be set to **20001**.  
