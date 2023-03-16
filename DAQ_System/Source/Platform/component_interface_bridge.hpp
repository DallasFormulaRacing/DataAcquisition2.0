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

#include "Adapter/Interfaces/ilinear_potentiometer.hpp"
#include "Adapter/LinearPotentiometer/linear_potentiometer.hpp"

#include "daq_pin_map.hpp"

namespace platform {

class ComponentInterfaceBridge {
    public:
        ComponentInterfaceBridge();
        ~ComponentInterfaceBridge();

    private:
        DaqPinMap pins;
        shared_ptr<adapter::ILinear_Potentiometer> linear_potentiometer_;
        

};


}


#endif // COMPONENT_INTERFACE_BRIDGE_H