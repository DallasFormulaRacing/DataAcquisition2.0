#include "Application/potentiometer.h"
#include "mbed.h"



// main() runs in its own thread in the OS
int main()
{
    Potentiometer p(PF_4);
    AnalogIn a(PF_4);

    while (true) {
        unsigned short d = a.read_u16();
        unsigned short e = p.read();
        printf ("[%d,%d]\n",d, e);
        //printf("%f \n", d);
    }
}

