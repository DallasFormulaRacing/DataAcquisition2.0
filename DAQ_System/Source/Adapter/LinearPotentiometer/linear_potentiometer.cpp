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

#include "linear_potentiometer.hpp"

namespace adapter {

LinearPotentiometer_SLS1322::LinearPotentiometer_SLS1322(PinName analog_pin) : analog_input_monitor_(analog_pin) { }
LinearPotentiometer_SLS1322::~LinearPotentiometer_SLS1322() { }

float LinearPotentiometer_SLS1322::GetDisplacementInches() {
    return displacement_percentage_ * kMaxLengthInches;
}

float LinearPotentiometer_SLS1322::GetDisplacementMillimeters() {
    return displacement_percentage_ * kMaxLengthMillimeters;
}

void LinearPotentiometer_SLS1322::ComputeDisplacementPercentage() {
    // The LinPot goes from 1-14k ohm, v = 3.3v, vdiv formula
    // Should compute values from 100 to 450

    //0 to 65535 is adc in range
    //      Kohm = (adcin * (V/steps) / Itot)
    float retraction_percentage = (float)(analog_input_monitor_.read_u16()) / kAnalogRange;
    displacement_percentage_ =  1 - retraction_percentage;
}

}
