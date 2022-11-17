#include "mbed.h"
#include "iostream"

// main() runs in its own thread in the OS
int main()
{
    CAN can(PD_1, PD_0, 250000);

    while (true) {
        CANMessage msg;

        can.read(msg);
        cout << msg.id << " ";

        for (char x : msg.data){
            cout << x << " ";
        }
        cout << endl;
    }
}

