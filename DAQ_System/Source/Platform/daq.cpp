/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include <iostream>

#include "daq.hpp"

// DFR Custom Dependancies
#include "platform/component_interface_bridge.hpp"

namespace platform {

DAQ::DAQ() { }

DAQ::~DAQ() { }

void DAQ::Init() {
    ComponentInterfaceBridge bridge;

    suspension_pots.front_left  = bridge.GetLinearPotentiometer(platform::front_left);
    suspension_pots.front_right = bridge.GetLinearPotentiometer(platform::front_right);
    suspension_pots.rear_left   = bridge.GetLinearPotentiometer(platform::rear_left);
    suspension_pots.rear_right  = bridge.GetLinearPotentiometer(platform::rear_right);
}

void DAQ::Read() {
    suspension_pots.front_left->ComputeDisplacementPercentage();
    suspension_pots.front_right->ComputeDisplacementPercentage();
    suspension_pots.rear_left->ComputeDisplacementPercentage();
    suspension_pots.rear_right->ComputeDisplacementPercentage();
}

void DAQ::Write(double timestamp) {
    std::cout << timestamp << " s\t"
              << suspension_pots.front_left->GetDisplacementInches() << " in\t"
              << suspension_pots.front_left->GetDisplacementMillimeters() << " mm" << std::endl;
}

}
