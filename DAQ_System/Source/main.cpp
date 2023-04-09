/*
* DataAcquisition2.0 — 2023 Vehicle
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

// C/C++ Standard Libraries
#include <iostream>
#include <memory>

// External Dependancies
#include "mbed.h"

// DFR Custom Dependancies
#include "Adapter/Interfaces/ilinear_potentiometer.hpp"
#include "platform/component_interface_bridge.hpp"

using AutoReloadTimer = LowPowerTicker;

static bool logging = false;
static void start_logging() {
    logging = true;
}

int main() {
    // Init
    platform::ComponentInterfaceBridge bridge;

    std::unique_ptr<adapter::ILinear_Potentiometer> linear_potentiometer = bridge.GetLinearPotentiometer();

    
    constexpr uint8_t kLoggingRate = 3;
    AutoReloadTimer timer;
    timer.attach(&start_logging, std::chrono::seconds(kLoggingRate));

    float timestamp = 0.0f;

    // Operate
    while (true) {
        linear_potentiometer->ComputeDisplacementPercentage();

        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            std::cout << linear_potentiometer->GetDisplacementPercentage() << "%" << std::endl;
            
            logging = false;
        }
    }
}
