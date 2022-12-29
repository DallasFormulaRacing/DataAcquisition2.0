#include "mbed.h"
#include "iostream"
#include "CAN3.h"
#include "Hx711.h"


// main() runs in its own thread in the OS
int main()
{
    /*
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
    }*/

    
    // PD_1 = RX, PD_0 = TX
    CAN can(PB_8, PB_9, 250000);
    
    CANMessage msg;



    while (true) {
        //int z = 5;
        //cout << endl << z << endl;
        can.read(msg);
        cout << msg.id << "\t";

        for (char x : msg.data){
            cout << x << " ";
        }
        cout << endl;
    }
    
}

