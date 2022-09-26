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
        int size;
        double vC;
        PinName pin;
};