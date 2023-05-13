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
    // TODO: When moving to later destroying this object at the end of booting up
    // Check that all private pointers are nullified and destroyed
}

std::unique_ptr<adapter::LinearPotentiometer_SLS1322> ComponentInterfaceBridge::GetLinearPotentiometer(LinPotLocation location) {
    PinName linpot_pin;
    switch (location) {
        case front_left:
            linpot_pin = pins.analog_linpot_front_left;
            break;
        case front_right:
            linpot_pin = pins.analog_linpot_front_right;
            break;
        case rear_left:
            linpot_pin = pins.analog_linpot_rear_left;
            break;
        case rear_right:
            linpot_pin = pins.analog_linpot_rear_right;
            break;
    }

    return std::make_unique<adapter::LinearPotentiometer_SLS1322>(linpot_pin);
}

std::unique_ptr<application::SdDataLogger> ComponentInterfaceBridge::GetSdDataLogger() {
    return std::make_unique<application::SdDataLogger>(
                pins.sd_mosi,
                pins.sd_miso,
                pins.sd_sck,
                pins.sd_cs);
}

}
