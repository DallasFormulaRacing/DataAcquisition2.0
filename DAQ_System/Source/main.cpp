#include <iostream>

#include "mbed.h"

#include "Adapter/LinearPotentiometer/linear_potentiometer.hpp"

// main() runs in its own thread in the OS
int main()
{
    LinearPotentiometer p(PF_4);
    AnalogIn a(PF_4);

    while (true) {
        unsigned short d = a.read_u16();
        double e = p.read();
        //printf ("[%d,%d]\n",d, e);
        cout << d << "\t" << e << endl;
        //printf("%f \n", d);
    }
}

