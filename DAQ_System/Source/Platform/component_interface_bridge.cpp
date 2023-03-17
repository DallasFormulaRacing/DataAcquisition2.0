/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include "component_interface_bridge.hpp"
#include <memory>

namespace platform {

ComponentInterfaceBridge::ComponentInterfaceBridge() { }

ComponentInterfaceBridge::~ComponentInterfaceBridge() {
    // Check that all private pointers are nullified and destroyed
    // Note: experiment on a side program if these remaining pointers are still valid after moving
}

std::unique_ptr<adapter::LinearPotentiometer> ComponentInterfaceBridge::GetLinearPotentiometer() {
    return std::make_unique<adapter::LinearPotentiometer>(pins.linpot_analog_input);
}



}


// Does the bridge have to be a class if we are only providing components?
//      If no class: a suite of getter platform::functions -- we do this regardless
//      So, class would be helpful to at least containerize the scope
//
// Consider returning the creation of unique pointers
//      Only one pointer is needed (to be managed by the FSM)
//      It is more efficient and simple
//      Will reduce need of Bridge private variables
//      https://www.lewuathe.com/return-std-make_unique-from-function.html 