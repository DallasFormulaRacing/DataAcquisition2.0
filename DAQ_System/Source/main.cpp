#include "mbed.h"
#include "iostream"

// main() runs in its own thread in the OS
int main()
{
    // PA_12 = TX, PA_11 = RX
    CAN can(PA_12, PA_11, 250000);
    CANMessage msg;
    while (true) {
        

        can.read(msg);
        cout << msg.id << "\t";

        for (char x : msg.data){
            cout << x << " ";
        }
        cout << endl;
    }
}

