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
#include "Platform/daq.hpp"

using AutoReloadTimer = LowPowerTicker;

static bool logging = false;
static void start_logging() {
    logging = true;
}



int main() {
    // Init components
    platform::DAQ daq;
    daq.Init();
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3; // seconds
    AutoReloadTimer timer;
    timer.attach(&start_logging, std::chrono::seconds(kLoggingRate));

    double timestamp = 0.0f;

    // Operate
    while (true) {
        daq.suspension_pots.front_left->ComputeDisplacementPercentage();

        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            std::cout << daq.suspension_pots.front_left->GetDisplacementInches() << " in\t"
                      << daq.suspension_pots.front_left->GetDisplacementMillimeters() << " mm" << std::endl;
            
            logging = false;
        }
    }
}
