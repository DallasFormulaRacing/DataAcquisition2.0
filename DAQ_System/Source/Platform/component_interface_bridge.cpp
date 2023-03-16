/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include "component_interface_bridge.hpp"

namespace platform {

ComponentInterfaceBridge::ComponentInterfaceBridge() {
    linear_potentiometer_ = make_shared<adapter::LinearPotentiometer>(pins.linpot_analog_input);
}

ComponentInterfaceBridge::~ComponentInterfaceBridge() {
    // Check that all private pointers are nullified and destroyed
    // Note: experiment on a side program if these remaining pointers are still valid after moving
}



}


// Does the bridge have to be a class if we are only providing components?
//      If no class: a suite of getter platform::functions -- we do this regardless
//      So, class would be helpful to at least containerize the scope and private pointers
//
//  Instead of returning & copying in main or FSM, consider moving:
//      https://stackoverflow.com/questions/41871115/why-would-i-stdmove-an-stdshared-ptr 