/*
Written by: Manuel DJC

implements AnalogSensor as a potentiometer with a coefficient.
*/
#include "mbed.h"
#include "AnalogIn.h"
#include "analogSensor.tpp"

class Potentiometer : public AnalogSensor {
    public:
        Potentiometer(PinName p, double vCoeff) : AnalogSensor(p) {
            vC = vCoeff;
        }

        Potentiometer(PinName p) : AnalogSensor(p) {
            vC = 1.0;
        }

        //sls1300 pot goes from 1-14k ohm
        ///reads potentiometer and converts to cm
        double read(){
            return AnalogIn(pin) * vC;
        }

        void changePin(PinName p){
            pin = p;
        }
        
        double getVCoeff(){
            return vC;
        }
    private:
        double vC;
        PinName pin;
};
