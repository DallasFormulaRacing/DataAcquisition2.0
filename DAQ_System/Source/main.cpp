#include "Application/potentiometer.h"
#include "mbed.h"
#include <iostream>



// main() runs in its own thread in the OS
int main()
{
    Potentiometer p(PF_4);
    AnalogIn a(PF_4);

    while (true) {
        unsigned short d = a.read_u16();
        double e = p.read();
        //printf ("[%d,%d]\n",d, e);
        cout << d << "\t" << e << endl;
        //printf("%f \n", d);
    }
}

