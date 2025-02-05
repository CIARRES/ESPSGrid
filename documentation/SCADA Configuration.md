# SCADA Configuration

Before running our model, the final step is to configure the SCADA system. This involves setting up the RTU as the associated data source and creating a variable in SCADA for each of the variables monitored by the RTU. Next, a visual representation (HMI) of the model needs to be designed.

The specifics of this step depend entirely on the project requirements and the components to be monitored and controlled, and thus we refer the reader to the comprehensive [tutorial](https://youtu.be/KrcL6lhAHKw) by Thiago Alves available online,  which outlines the steps for connecting the RTU to the SCADA system.

Nevertheless, to enable replication and facilitate the experimentation with our model, we provide a pre-configured JSON file in our repository. This file can be imported directly into the SCADA system, recreating the configuration and HMI used in our example setup.
