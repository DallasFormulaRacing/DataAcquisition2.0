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

struct SuspensionPotentiometers {
    std::unique_ptr<adapter::ILinear_Potentiometer> front_left;
    std::unique_ptr<adapter::ILinear_Potentiometer> front_right;
    std::unique_ptr<adapter::ILinear_Potentiometer> rear_left;
    std::unique_ptr<adapter::ILinear_Potentiometer> rear_right;
};

int main() {
    // Init components
    platform::ComponentInterfaceBridge bridge;

    SuspensionPotentiometers suspension_pots;
    suspension_pots.front_left  = bridge.GetLinearPotentiometer(platform::front_left);
    suspension_pots.front_right = bridge.GetLinearPotentiometer(platform::front_right);
    suspension_pots.rear_left   = bridge.GetLinearPotentiometer(platform::rear_left);
    suspension_pots.rear_right  = bridge.GetLinearPotentiometer(platform::rear_right);
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3;
    AutoReloadTimer timer;
    timer.attach(&start_logging, std::chrono::seconds(kLoggingRate));

    float timestamp = 0.0f;

    // Operate
    while (true) {
        suspension_pots.front_left->ComputeDisplacementPercentage();

        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            std::cout << suspension_pots.front_left->GetDisplacementPercentage() << "%" << std::endl;
            
            logging = false;
        }
    }
}

/*
* todo

rename linpot class with sensor model name
rename get displacement to get retraction
add get displacement percentage (simply do 1 minus retraction total)
add get mm
add get inches

*/
