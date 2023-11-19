# `DataAcquisition2.0`
🏁 For documentation, checkout our [Wiki](https://github.com/DallasFormulaRacing/DataAcquisition2.0/wiki)!

## Table of Contents
1. [Objective](#objective)
2. [User Features](#user-features)
3. [Hardware and Equipment](#hardware-and-equipment)
4. [Firmware Tech Stack](#firmware-tech-stack)
5. [Automation](#automation)
6. [Future Plans](#future-plans)
7. [Deprecated Technologies](#deprecated-technologies)

## Objective:
- A Data Acquisition (DAQ) System for interfacing with the vehicle ECU and additional sensors.
- Build the first fully-fledged DAQ with re-usable code through:
    - The Layered Architecture design pattern
    - Modularity of components
    - Hardware abstraction
- Efficiently record data to a CSV file for system testing & refinement of other vehicle components via analyses.
- Make sensor data conveniently accessible for the whole DFR organization.
- To be used in tandem with DFR's [DataAnalyzer](https://github.com/DallasFormulaRacing/DataAnalyzer).

## User Features
| Supported Logging Method | Library Completion    |
| :----------------------- | :---------------------|
| Micro-SD Card            | <span>&#10004;</span> |
| USB Flash Drive          |                       |
| Wireless Transmission    |                       |

## Hardware and Equipment
### Target Device
The [STM32 Nucleo F429ZI](https://www.st.com/en/evaluation-tools/nucleo-f429zi.html) microcontroller development board is used as the current target device for code merged to the `main` and `develop` branches. Theoretically, other Arm Cortex M4 based STM32 devices with the necessary peripherals should be able to run the same program.

![image](https://github.com/DallasFormulaRacing/DataAcquisition2.0/assets/71054319/00c8cfe3-c1f8-499c-8c72-fe8f191a8108)

### Input
| Component Type        | Supported Hardware Device                                                                                            | Library Completion    |
| :-------------------- |:-------------------------------------------------------------------------------------------------------------------- | :---------------------|
| Linear Potentiometer  | - [SLS 1322-3](https://www.activesensors.com/products/linear-position-sensors/general-purpose-linear-potentiometers) | <span>&#10004;</span> |
| Accelerometer         | - [LSM303DLHC](https://www.adafruit.com/product/1714)                                                                | <span>&#10004;</span> |
| Gyroscope             | - [L3GD20H](https://www.adafruit.com/product/1714)                                                                   | <span>&#10004;</span> |
| Magnetometer          | - [L3GD20H](https://www.adafruit.com/product/1714)                                                                   | <span>&#10004;</span> |
| GPS/GNSS              | - [NEO-M9N](https://www.sparkfun.com/products/15733)                                                                 |                       |
| Strain Gauges         |                                                                                                                      |                       |
| Tire Temperature      | - [MLX90614](https://www.sparkfun.com/products/9570)                                                                 |                       |
| ECU                   | - [PE3-8400P](https://pe-ltd.com/product/pe3-8400p/)                                                                 |                       |

For additional information regarding each device and its implemention, refer to our [Wiki](https://github.com/DallasFormulaRacing/DataAcquisition2.0/wiki).

### Output
A CSV file with associate a timestamp with the following sampled data.

| Computed Output                | Supported Units                                         |
| :----------------------------- |:------------------------------------------------------- |
| Suspension Travel Displacement | - Inches <br /> - Millimeters                           |
| Acceleration (3 DoF)           | - m/s^2 <br /> - Gs                                     |
| Rotational Velocity (3 DoF)    | - Degrees per second <br /> - Radians per second        |
| Location Coordinates           | - Altitude <br /> - Latitude <br /> - Longitude         |
| External Force                 | - Stress <br /> - Torque <br /> - Pressure              |
| Tire Temperature               | - Farenheit <br /> - Celsius                            |
| Motor Shaft Speed              | - Revolutions per Minute (RPM)                          |
| Throttle Position              | - Percentage                                            |
| Fuel Open Time                 | - Milliseconds                                          |
| Ignition Angle                 | - Degrees                                               |
| Atmospheric Pressure           | - Pound per Square Inch (PSI) <br /> - Kilopascal (kPa) |
| Intake Manifold Pressure       | - Pound per Square Inch (PSI) <br /> - Kilopascal (kPa) |
| Exhaust Oxygen                 | - Lambda                                                |


## Firmware Tech Stack
- STM32 Hardware Abstraction Layer ([HAL](https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf))
  - Generated by STM32CubeMX within the CubeIDE environment, which also provides the `arm-none-eabi` toolchain.
  - Serves as a set of drivers or a Board Support Package (BSP) to interface with the microcontroller.
- [FreeRTOS](https://www.freertos.org/index.html)
  - Multi-threading and concurrent programming.
  - For optimizing timing and performance.
- [CMSIS-RTOS v2](https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS.html)
  - Not to be mistaken for another RTOS framework.
  - An abstraction layer for interacting with an RTOS framework (i.e., FreeRTOS).
- [FatFS](http://elm-chan.org/fsw/ff/)
  - For maintaining the FAT file system.
  - Allows users to access the resulting CSV file on any device

## Automation
- Unit tests are developed using [Google Test](https://google.github.io/googletest/) and are maintained in `DAQ_System/Tests`.
- See our [Unit Testing page](https://github.com/DallasFormulaRacing/DataAcquisition2.0/wiki/Unit-Testing) for running them locally by leveraging [Docker](https://docs.docker.com/get-started/overview/).
- Continuous Integration (CI)
  - The firmware is compiled as an stm32CubeIDE project using the following GitHub Action: [xanderhendriks/action-build-stm32cubeide](https://github.com/marketplace/actions/build-stm32cubeide-project).
  - The unit tests are compiled and ran using [CMake](https://cmake.org/cmake/help/latest/index.html) with [Ninja](https://ninja-build.org/manual.html). 


## Future Plans
- Developing for a dual-core STM32H7 microcontroller.
  - The Cortex M4 core may focus on sampling data from sensors.
  - The Cortex M7 core may focus on outputting the processed data.
  - Both cores may run FreeRTOS for organizational purposes.
  - Both cores may communicate and synchronize with each other using the OpenAMP framework.
- Developing on a custom PCB microcontroller.
  - Currently in progress and led by our Electrical Team.
  - Ideal for mounting on the vehicle.


## Deprecated Technologies
Initially, this project ran a program consisting of a simple super loop and interfaced with the hardware using the Mbed framework. This allowed us to focus on building the team knowledge base for developing custom drivers to support sensors. In favor of our long term goals, we have transitioned to supporting the above [Firmware Tech Stack](#firmware-tech-stack) for the DAQ.
