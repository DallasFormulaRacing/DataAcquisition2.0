/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef COMPONENT_INTERFACE_BRIDGE_H
#define COMPONENT_INTERFACE_BRIDGE_H

// C/C++ Standard Libraries
#include <memory>

// DFR Custom Dependancies
#include "Adapter/LinearPotentiometer/linear_potentiometer_sls1322.hpp"
#include "Application/DataLogger/sd_data_logger.hpp"
#include "daq_pin_map.hpp"



namespace platform {

enum LinPotLocation {front_left, front_right, rear_left, rear_right};

class ComponentInterfaceBridge {
    public:
        ComponentInterfaceBridge();
        ~ComponentInterfaceBridge();

        std::unique_ptr<adapter::LinearPotentiometer_SLS1322> GetLinearPotentiometer(LinPotLocation location);

        std::unique_ptr<application::SdDataLogger> GetSdDataLogger();

    private:
        DaqPinMap pins;
};

}

#endif // COMPONENT_INTERFACE_BRIDGE_H