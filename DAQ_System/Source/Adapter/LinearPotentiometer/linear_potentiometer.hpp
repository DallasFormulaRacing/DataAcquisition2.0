/*
* Linear Potentiometer
* Author:        Manuel DJC              
* Email:         Manuel.DeJesusContreras@UTDallas.edu
* 
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef POTENTOMETER_H
#define POTENTOMETER_H

#include "mbed.h"

class LinearPotentiometer {
    public:
        LinearPotentiometer(PinName p);
        double read();
        void setPin(PinName p);
        double getLast();
    private:
        AnalogIn ain_;
        double last_;
};

#endif