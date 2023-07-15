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

#ifndef LINEAR_POTENTOMETER_SLS1322_H
#define LINEAR_POTENTOMETER_SLS1322_H

#include "mbed.h"

#include "Adapter/Interfaces/ilinear_potentiometer.hpp"
#include <cstdint>


namespace adapter {

class LinearPotentiometer_SLS1322 : public adapter::ILinear_Potentiometer {
    public:
        LinearPotentiometer_SLS1322(PinName analog_pin);
        virtual ~LinearPotentiometer_SLS1322();

        float GetDisplacementInches() override;
        float GetDisplacementMillimeters() override;

        void ComputeDisplacementPercentage() override;

    private:
        AnalogIn analog_input_monitor_;
        float displacement_percentage_;

        // Mbed AnalogIn range: 2^(16 bits)
        // Note: hardware adc is 12 bit (mbed provides padding via AnalogIn)
        static constexpr uint16_t kAnalogRange = 65535;
        static constexpr uint8_t kMaxLengthInches = 3;
        static constexpr float kMaxLengthMillimeters = 25.4f * kMaxLengthInches;
};

}

#endif // LINEAR_POTENTOMETER_SLS1322_H