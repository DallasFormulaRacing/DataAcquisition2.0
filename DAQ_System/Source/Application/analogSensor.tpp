/*
Written by: Manuel DJC

Class to be implemented by different analog sensors
*/
#include "mbed.h"
#include <string>

class AnalogSensor{
    public:
        AnalogSensor(PinName pin){
            this->pin = pin;
        }
        //stm32 adc goes from 0-4095 same as 0-3.3v
        ///Read returns a double in the units being measured 
        virtual double read() = 0;
        virtual PinName getPinName(){
            return pin;
        };

    private:
        PinName pin;
};