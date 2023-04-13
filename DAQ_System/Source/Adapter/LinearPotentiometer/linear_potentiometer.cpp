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

void LinearPotentiometer_SLS1322::SetPin(PinName analog_pin) {
    analog_input_monitor_ = AnalogIn(analog_pin);
}


double LinearPotentiometer_SLS1322::GetDisplacementPercentage() {
    return displacement_percentage_;
}


void LinearPotentiometer_SLS1322::ComputeDisplacementPercentage() {
    // The LinPot goes from 1-14k ohm, v = 3.3v, vdiv formula
    // Should compute values from 100 to 450

    //0 to 65535 is adc in range
    //      Kohm = (adcin * (V/steps) / Itot)
    displacement_percentage_ = (double)(analog_input_monitor_.read_u16()) / kAnalogRange;
}

}
