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
#include <memory>


/*
* Where should we keep this enum class?
*   Application: Because this is vague
*   Platform: Because for this iteration of the daq, we are supporting the following linpots
* This needs to be shared with main.cpp or the envisioned DAQ class
* The enum keywords themselves have nothing to do with the platform -- it's an application decision
* On the other hand, the enum class is an argument of the INTERFACE for platform components (i.e., the pins)
*/


namespace platform {

enum LinPotLocation {front_left, front_right, rear_left, rear_right};

class ComponentInterfaceBridge {
    public:
        ComponentInterfaceBridge();
        ~ComponentInterfaceBridge();

        std::unique_ptr<adapter::LinearPotentiometer> GetLinearPotentiometer(LinPotLocation location);

    private:
        DaqPinMap pins;
};


}


#endif // COMPONENT_INTERFACE_BRIDGE_H