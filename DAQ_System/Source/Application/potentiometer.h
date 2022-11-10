/*
Written by: Manuel DJC
header file for potentiometer class
*/
#ifndef POTENTOMETER_H
#define POTENTOMETER_H
#include "mbed.h"

class Potentiometer {
    public:
        Potentiometer(PinName p);
        double read();
        void setPin(PinName p);
        double getLast();
    private:
        AnalogIn ain_;
        double last_;
};

#endif