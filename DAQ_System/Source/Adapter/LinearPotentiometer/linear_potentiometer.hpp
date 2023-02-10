/*
* Linear Potentiometer SLS1300
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
        LinearPotentiometer(PinName analog_pin);
        virtual ~LinearPotentiometer();
        void SetPin(PinName analog_pin);


        double GetDisplacementPercentage() override;
        void ComputeDisplacementPercentage() override;
    private:
        AnalogIn analog_input_monitor_;
        double displacement_percentage_;

        // Mbed AnalogIn range: 2^(16 bits)
        // Note: hardware adc is 12 bit
        const int kAnalogRange = 65535;
};

}

#endif // LINEAR_POTENTOMETER_H