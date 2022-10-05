/*
Written by: Manuel DJC

implements AnalogSensor as a potentiometer with a coefficient.
*/
#include "mbed.h"
#include "AnalogIn.h"
#include "analog_sensor.tpp"

class Potentiometer : public AnalogSensor {
    public:
        Potentiometer(PinName p, string name, string units, double vCoeff) : AnalogSensor(p, name, units) {
            vC_ = vCoeff;
        }

        Potentiometer(PinName p, string name, string units) : AnalogSensor(p, name, units) {
            vC_ = 1.0;
        }

        //sls1300 pot goes from 1-14k ohm, v = 3.3v, vdiv formula. Should return values from 100 to 450
        
        ///reads potentiometer and converts to mm
        double read(){
            //      Kohm = (adcin * (V/steps) / Itot)

            return 25*(AnalogIn(pin_)*(3.3/4096)/.2357143) * vC_ + 100;
        }

        void changePin(PinName p){
            pin_ = p;
        }
        
        double getVCoeff(){
            return vC_;
        }
    private:
        double vC_;
        PinName pin_;
};
