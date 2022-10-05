/*
Written by: Manuel DJC
header file for potentiometer class
*/
#include "mbed.h"

class Potentiometer {
    public:
        Potentiometer(PinName p);
        double read();
        void changePin(PinName p);
        PinName getPinName();
        double getLast();
    private:
        PinName pin_;
        double last_;
};