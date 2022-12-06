
#ifndef _CAN3_H_
#define _CAN3_H_

#include "mbed.h"
#include "mcp2515.h"
#include "mcp2515_defs.h"
#include "mcp2515_can.h"
#include "SPI.h"

#define BFPCTRL        0x0C
#define TXRTSCTRL       0x0D


class CAN3  {
    SPI& spi;
    //InterruptIn _itr;
    mcp2515 _mcp;
public:
    CAN3(SPI& _spi, PinName ncs);//, PinName itr);
    
    uint8_t read(CANMessage *msg); //    int read(CANMessage&    msg);
    void write(CANMessage* test);   //int write(CANMessage     msg);
    int frequency( int canSpeed); //Set the frequency of the CAN interface hz    The bus frequency in hertz


    void reset(); //Reset CAN interface. To use after error overflow.
    unsigned char rderror(); //Returns number of read errors to detect read overflow errors.
    unsigned char tderror(); //Returns number of write errors to detect write overflow errors.

    void rise(void (*fptr)(void));
    void fall(void (*fptr2)(void));

    // Private functions
private :
    uint8_t checkReceive(void);


};

#endif // _CAN3_H_
