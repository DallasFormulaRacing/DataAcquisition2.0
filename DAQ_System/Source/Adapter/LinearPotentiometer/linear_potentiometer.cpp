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

#include "AnalogIn.h"
#include "mbed.h"

#include "linear_potentiometer.hpp"

namespace adapter {

LinearPotentiometer::LinearPotentiometer(PinName p) : ain_(p) { }
LinearPotentiometer::~LinearPotentiometer() { }

void LinearPotentiometer::setPin(PinName p){
    ain_ = AnalogIn(p);
}


double LinearPotentiometer::getLast(){
    return last_;
}

//sls1300 pot goes from 1-14k ohm, v = 3.3v, vdiv formula. Should return values from 100 to 450
///reads potentiometer and converts to mm
double LinearPotentiometer::ReadDisplacementPercentage() {
    //0 to 65535 is adc in range
    //      Kohm = (adcin * (V/steps) / Itot)
    last_ = (double)(ain_.read_u16()) / 65535.0;
    //last_ = 25*ain_.read_u16()*(3.3/65535)/.235;
    return last_;
}

PinName pin_;
double last_;

}
