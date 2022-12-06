#include "mbed.h"
#include "iostream"
#include "CAN3.h"


// main() runs in its own thread in the OS
int main()
{
    SPI spi(PE_6, PE_5, PE_2);// MOSI, MISO, SCLK : on arduino/shield: D11, D12, D13
    PinName chipSelect = PE_4; // CS, D10 on arduino/shield
    CAN3 can(spi, chipSelect);

    can.frequency(250000);
    while (true) {
        CANMessage *msg;

        can.read(msg);
        cout << msg->id << " ";

        for (char x : msg->data){
            cout << x << " ";
        }
        cout << endl;
    }
    /*
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
    */
}

