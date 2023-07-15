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
#include <cstdio>

#include "gyroscope_l3gd20h.hpp"

std::unique_ptr<adapter::IGyroscope> gyro = std::make_unique<adapter::Gyroscope_L3GD20H>(I2C_SDA,I2C_SCL);


int main(){
    // for the 3 arrays below the first element will deal with purely the x-axis the second element is y and the last element is z. These will be used to calculate an average
    // that will be used for the offeset for the callibration of the gyroscope. Int n is used to control the sample size for the average.
    short degrees_per_second[3] = {0};
    float radians_per_second[3] = {0};
    // logical conditions to determine the branches that will exeute in the code
    bool offsetStatus = false;
    
    while (true){
        if( gyro->ComputeDegreesPerSecond(degrees_per_second) && gyro->ComputeRadiansPerSecond(radians_per_second)){
            
            printf("Degrees Per Second: X = %d, Y = %d, Z = %d", degrees_per_second[0], degrees_per_second[1], degrees_per_second[2]);
            printf("   ");
            printf("Radians per second =  X = %f, Y = %f, Z = %f  \n", radians_per_second[0],radians_per_second[1], radians_per_second[2]);
        }
        else {
            printf( "It's not Gyroin Time =( \n");
        }
    }             
}



