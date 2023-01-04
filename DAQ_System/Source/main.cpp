#include <iostream>
#include <memory>

#include "mbed.h"

// This file should not use hardware-dependent includes
// TODO: A component that serves component assembly using abstract classes
#include "Adapter/Interfaces/ilinear_potentiometer.hpp"
#include "Adapter/LinearPotentiometer/linear_potentiometer.hpp"

// main() runs in its own thread in the OS
int main() {
    // LinearPotentiometer p(PF_4);
    // AnalogIn a(PF_4);

    // while (true) {
    //     unsigned short d = a.read_u16();
    //     double e = p.read();
    //     //printf ("[%d,%d]\n",d, e);
    //     cout << d << "\t" << e << endl;
    //     //printf("%f \n", d);
    // }

    shared_ptr<adapter::ILinear_Potentiometer> linear_potentiometer = make_shared<adapter::LinearPotentiometer>(PF_4);
    AnalogIn a(PF_4);

    while (true) {
        unsigned short d = a.read_u16();
        double displacement_mm = linear_potentiometer->read_displacement();
        std::cout << d << "\t" << displacement_mm << std::endl;
    }
}

