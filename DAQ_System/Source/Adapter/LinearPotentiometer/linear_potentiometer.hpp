/*
* Linear Potentiometer
* Author:        Manuel DJC    
* Modified By:   Cristian Cruz
*          
* Email:         Manuel.DeJesusContreras@UTDallas.edu
*                cris14.alex@gmail.com
* 
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef LINEAR_POTENTOMETER_H
#define LINEAR_POTENTOMETER_H

#include "mbed.h"

#include "Adapter/Interfaces/ilinear_potentiometer.hpp"

namespace adapter {

class LinearPotentiometer : public adapter::ILinear_Potentiometer {
    public:
        LinearPotentiometer(PinName p);
        virtual ~LinearPotentiometer();
        void setPin(PinName p);
        double getLast();

        double read_displacement() override;
    private:
        AnalogIn ain_;
        double last_;
};

}

#endif // LINEAR_POTENTOMETER_H