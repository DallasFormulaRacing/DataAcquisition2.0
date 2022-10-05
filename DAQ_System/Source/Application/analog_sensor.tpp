/*
Written by: Manuel DJC

Class to be implemented by different analog sensors
*/
#include "mbed.h"
#include <string>
#include <iostream>

class AnalogSensor{
    public:
        AnalogSensor(PinName pin, string name, string units){
            this->pin_ = pin;
            this->name_ = name;
            this->units_ = units;
        }
        //stm32 adc goes from 0-4095 same as 0-3.3v. To convert to voltage, ADCIN*(3.3/4096)

        ///Read returns a double in the units being measured 
        virtual double read() = 0;
        ///returns raw analog in value
        virtual double readRaw(){
            return AnalogIn(pin_);
        }
        virtual PinName getPinName(){
            return pin_;
        };

        friend ostream& operator<<(ostream& out, const AnalogSensor& sensor);

    private:
        PinName pin_;
        string units_;
        string name_;
};

ostream& operator<<(ostream& os, const AnalogSensor& sensor){
    os << sensor.name_ << "," << sensor.units_ << ",";
    return os;
}