# RTU Configuration

After deploying the RTU, it must be configured. This process is relatively complex, as it requires uploading the ladder logic program (in `.st` format) to define the variables monitored by the RTU, as well as the IED configuration files. These last files, written in Substation Configuration Language ([SCL](https://www.igrid-td.com/smartguide/iec61850/scl-substation-configuration-language/)), specify the data attributes of the IEDs. Since creating these files can be tedious and IEDs of the same type often differ only in a few configurations, we provide a script in the repository to simplify the generation of SCL files for power meters and circuit breakers. Users can specify the desired `id` and `IP` address during the generation process. Once they are created, these files must be uploaded to the directory path `/workdir/webserver/scl_client_files/` within the RTU container.

For the `.st` program, we recommend using the OpenPLC Editor, which includes a detailed guide available [online](https://autonomylogic.com/docs/). The IEDs variables, using the names defined in the SCL files, and their corresponding physical addresses in the RTU must be defined as follows:

- **For each power meter:**
  - `PM{id}VA`, `PM{id}VB`, `PM{id}VC`: These represent the voltage readings. Each variable is of type `WORD` (16 bits) and should be mapped to an `%IW` register.
  - `PM{id}IA`, `PM{id}IB`, `PM{id}IC`: These represent the current readings and, like the voltage readings, must be mapped to an `%IW` register.
  - `PM{id}SecST`: This variable represents the safety status calculated by the power meter. It is a boolean variable and should be mapped to an input register `%IX` for a single bit.
  
- **For each circuit breaker:**
  - `CB{id}CST`: This variable represents the device's current state (open or closed). It is a boolean variable and should be mapped to an input register `%IX` for a single bit.
  - `CB{id}OpST`: This variable corresponds to the operator's command. It is also a boolean variable and should be mapped to an output register `%QX` for a single bit.

`{id}` refers to the identifier of the IED, formatted as a two-digit number.

Once the SCL files are loaded and the program in `.st` format has been generated, the next step is to upload the program to the RTU, compile it, and launch it.
