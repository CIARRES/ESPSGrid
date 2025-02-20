# Deployment

The deployment of the local network, IEDs, and control systems is managed through Docker Compose. To achieve this, we create a YAML file where we define the devices to be deployed, along with their parameters and IP addresses.

- **PM_ID** and **CB_ID**: the IDs of the power meter and circuit breaker, respectively.
- **PM_GATEWAY** and **CB_GATEWAY**: the IP addresses of the router to which the IEDs are connected.
- **S_IP**: the IP address of the machine running the MATLAB/Simulink physical model.
- **S_PORT**: the UDP port number where the corresponding Simulink block is transmitting (for the power meter) or listening (for the circuit breaker).

```yaml
services:

  rtu:
    image: javiersande/sg-rtu:latest
    tty: true
    networks:
      transportNetwork:
        ipv4_address: 10.0.0.3

  scada:
    image: javiersande/sg-scada:latest
    tty: true
    networks:
      transportNetwork:
        ipv4_address: 10.0.0.4

  powermeter1:
    image: javiersande/sg-powermeter:latest
    environment:
      - PM_ID=1
      - PM_IP=10.0.1.1
      - PM_GATEWAY=10.0.0.1
      - S_IP=10.0.0.2
      - S_DATA_PORT=10001
      - SAFE_THRESHOLD=10000
    networks:
      transportNetwork:
        ipv4_address: 10.0.1.1

  circuitbreaker1:
    image: javiersande/sg-circuitbreaker:latest
    environment:
      - CB_ID=1
      - CB_IP=10.0.1.2
      - CB_GATEWAY=10.0.0.1
      - S_IP=10.0.0.2
      - S_PORT=20001
      - AUTO_MODE=1
    networks:
      transportNetwork:
        ipv4_address: 10.0.1.2


# Omitted power meters 2, 3, 4 and circuit breakers 2, 3, 4 for brevity

networks:
  transportNetwork:
    driver: ipvlan
    driver_opts:
      parent: eth0
      ipvlan_mode: l2
    ipam:
      config:
        - subnet: 10.0.0.0/16
          gateway: 10.0.0.1
```
