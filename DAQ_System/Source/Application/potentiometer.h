/*
Written by: Manuel DJC

this file is probably no longer needed
*/
#include "mbed.h"

class Potentiometer {
    public:
        Potentiometer(PinName p, double vCoeff);
        Potentiometer();
        double read();
        void changePin(PinName p);
        double getVCoeff();
        PinName getPinName();
    private:
        double vC;
        PinName pin;
};