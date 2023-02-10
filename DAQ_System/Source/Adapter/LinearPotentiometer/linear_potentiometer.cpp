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

#include "linear_potentiometer.hpp"

namespace adapter {

LinearPotentiometer::LinearPotentiometer(PinName analog_pin) : analog_input_monitor_(analog_pin) { }
LinearPotentiometer::~LinearPotentiometer() { }

void LinearPotentiometer::SetPin(PinName analog_pin) {
    analog_input_monitor_ = AnalogIn(analog_pin);
}


double LinearPotentiometer::GetDisplacementPercentage() {
    return displacement_percentage_;
}


void LinearPotentiometer::ComputeDisplacementPercentage() {
    //sls1300 pot goes from 1-14k ohm, v = 3.3v, vdiv formula. Should compute values from 100 to 450
    //reads potentiometer and converts to mm

    //0 to 65535 is adc in range
    //      Kohm = (adcin * (V/steps) / Itot)
    displacement_percentage_ = (double)(analog_input_monitor_.read_u16()) / 65535;
    //last_ = 25*ain_.read_u16()*(3.3/65535)/.235;
}

}
