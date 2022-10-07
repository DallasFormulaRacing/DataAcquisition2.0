/*
Written by: Manuel DJC

implements AnalogSensor as a potentiometer with a coefficient.
*/
#include "mbed.h"
#include "AnalogIn.h"

class Potentiometer {
    public:
        Potentiometer(PinName p) {
            pin_ = p;
        }
        //sls1300 pot goes from 1-14k ohm, v = 3.3v, vdiv formula. Should return values from 100 to 450
        
        ///reads potentiometer and converts to mm
        double read(){
            //      Kohm = (adcin * (V/steps) / Itot)
            last_ = 25*AnalogIn(pin_)*(3.3/4096)/.235+100;
            return last_;
        }

        void changePin(PinName p){
            pin_ = p;
        }

        PinName getPinName(){
            return pin_;
        }

        double getLast(){
            return last_;
        }

    private:
        PinName pin_;
        double last_;
};
