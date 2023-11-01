// /*
// * DataAcquisition2.0 — 2023 Vehicle
// * (c) 2022 Dallas Formula Racing - Embedded Firmware Team
// * Formula SAE International Collegiate Chapter
// * GPL-3.0 License
// */

// // C/C++ Standard Libraries
// #include <iostream>
// #include <memory>

// // External Dependancies
// #include "mbed.h"

// // DFR Custom Dependancies
// #include "Adapter/Interfaces/ilinear_potentiometer.hpp"
// #include "Platform/component_interface_bridge.hpp"

// using AutoReloadTimer = LowPowerTicker;

// static bool logging = false;
// static void start_logging() {
//     logging = true;
// }

// struct SuspensionPotentiometers {
//     std::unique_ptr<adapter::ILinear_Potentiometer> front_left;
//     std::unique_ptr<adapter::ILinear_Potentiometer> front_right;
//     std::unique_ptr<adapter::ILinear_Potentiometer> rear_left;
//     std::unique_ptr<adapter::ILinear_Potentiometer> rear_right;
// };

// int main() {
//     // Init components
//     platform::ComponentInterfaceBridge bridge;

//     SuspensionPotentiometers suspension_pots;
//     suspension_pots.front_left  = bridge.GetLinearPotentiometer(platform::front_left);
//     suspension_pots.front_right = bridge.GetLinearPotentiometer(platform::front_right);
//     suspension_pots.rear_left   = bridge.GetLinearPotentiometer(platform::rear_left);
//     suspension_pots.rear_right  = bridge.GetLinearPotentiometer(platform::rear_right);
    
//     // Start timer
//     constexpr uint8_t kLoggingRate = 3; // seconds
//     AutoReloadTimer timer;
//     timer.attach(&start_logging, std::chrono::seconds(kLoggingRate));

//     double timestamp = 0.0f;

//     // Operate
//     while (true) {
//         suspension_pots.front_left->ComputeDisplacementPercentage();

//         if (logging) {
//             // Operate: Writing
//             timestamp += kLoggingRate;
//             std::cout << suspension_pots.front_left->GetDisplacementInches() << " in\t"
//                       << suspension_pots.front_left->GetDisplacementMillimeters() << " mm" << std::endl;
            
//             logging = false;
//         }
//     }
// }


//==================================================================================

#include "mbed.h"

// DFR Custom Dependancies
#include "Application/DAQ/daq.hpp"

using AutoReloadTimer = LowPowerTicker;

static InterruptIn button(PC_13);
static bool data_logging_enable = false;
static bool logging = false;

static void logging_signal() {
    logging = true;
}

// interrupt routine activated by a falling edge of button input
void ToggleDataLogging(void) {
    data_logging_enable = !data_logging_enable;
}

int main() {
    // Init components
    application::DAQ daq;
    daq.Init();
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3; // seconds
    AutoReloadTimer timer;
    timer.attach(&logging_signal, std::chrono::seconds(kLoggingRate));

    // use internal pullup for pushbutton 
    button.mode(PullUp);
    wait_ns(100);
    button.fall(&ToggleDataLogging);

    uint8_t open_file = 0;
    float timestamp = 0.0f;


    printf("Entering Super Loop\n");
    // Operate
    while (true) {
        daq.Read();

        // TODO: De-couple this condition. It should not take 3 to close the file
        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            daq.Write(timestamp, data_logging_enable);
            logging = false;
        }
    }
}
