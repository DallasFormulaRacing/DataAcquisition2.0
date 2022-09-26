#include "mbed.h"
#include "AnalogIn.h"
#include "potentiometer.h"

Potentiometer::Potentiometer(PinName p, double vCoeff) {
    pin = p;
    vC = vCoeff;
}

Potentiometer::Potentiometer() {
    pin = PA_0;
    vC = 1.0;
}

void Potentiometer::changePin(PinName p){
    pin = p;
}

double Potentiometer::read(){
    return AnalogIn(pin) * vC;
}

double Potentiometer::getVCoeff(){
    return vC;

}
PinName Potentiometer::getPinName(){
    return pin;
}

