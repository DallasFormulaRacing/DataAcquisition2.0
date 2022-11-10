/*
Written by: Manuel DJC

implements AnalogSensor as a potentiometer with a coefficient.
*/
#include "potentiometer.h"
#include "AnalogIn.h"
#include "mbed.h"

Potentiometer::Potentiometer(PinName p) : ain_(p) {
}
//sls1300 pot goes from 1-14k ohm, v = 3.3v, vdiv formula. Should return values from 100 to 450

///reads potentiometer and converts to mm
double Potentiometer::read(){
    //0 to 65535 is adc in range
    //      Kohm = (adcin * (V/steps) / Itot)
    last_ = (double)(ain_.read_u16())/65535.0;
    //last_ = 25*ain_.read_u16()*(3.3/65535)/.235;
    return last_;
}

void Potentiometer::setPin(PinName p){
    ain_ = AnalogIn(p);
}


double Potentiometer::getLast(){
    return last_;
}

PinName pin_;
double last_;
