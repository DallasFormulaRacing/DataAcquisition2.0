#include "CAN3.h"

#include "mbed.h"
#include "mcp2515.h"
#include "mcp2515_can.h"
#include "mcp2515_defs.h"


CAN3::CAN3(SPI& _spi, PinName ncs)//, PinName itr)
        : spi(_spi), _mcp(spi, ncs) {//, _itr(itr) {
    printf("\n\rcan = %d",this);

}

uint8_t CAN3::read(CANMessage *msg) {
    uint8_t stat, res;

    stat = _mcp.readStatus();

    if ( stat & MCP_STAT_RX0IF ) {
        // Msg in Buffer 0
        _mcp.read_canMsg( MCP_RXBUF_0, msg);
        _mcp.modifyRegister(MCP_CANINTF, MCP_RX0IF, 0);
        res = CAN_OK;
    } else if ( stat & MCP_STAT_RX1IF ) {
        // Msg in Buffer 1
        _mcp.read_canMsg( MCP_RXBUF_1, msg);
        _mcp.modifyRegister(MCP_CANINTF, MCP_RX1IF, 0);
        res = CAN_OK;
    } else {
        res = CAN_NOMSG;
    }

    return res;
}

uint8_t CAN3::checkReceive(void) {
    uint8_t res;

    res = _mcp.readStatus(); // RXnIF in Bit 1 and 0
    if ( res & MCP_STAT_RXIF_MASK ) {
        return CAN_MSGAVAIL;
    } else {
        return CAN_NOMSG;
    }
}

void CAN3::write(CANMessage* test) {
    uint8_t  txbuf_n;
    _mcp.getNextFreeTXBuf(&txbuf_n);
    _mcp.write_canMsg(txbuf_n,test);
    _mcp.start_transmit( txbuf_n );
}

void CAN3::rise(void (*fptr)(void)) {
//    _itr.rise(fptr);
}

void CAN3::fall(void (*fptr2)(void)) {
//    _itr.fall(fptr2);
}

int CAN3::frequency(int canSpeed) {

    uint8_t res;

    res = _mcp.init(canSpeed); //CAN_500KBPS_8MHZ
    wait(.001);

    _mcp.setRegister(MCP_CANINTE, 0x3);//0x3); //MCP_RX_INT);
    _mcp.setRegister(MCP_CANINTF, 0x3);// 0xff);


//RX0,1 as rx0,1 digital interrupt outputs
//_mcp.setRegister(BFPCTRL, 0xf);

//[Set TX0,1,2 as digital inputs
//_mcp.setRegister(TXRTSCTRL, 0x0);

    // printf("Setting Normal-Mode - \n\r ");
    if ( _mcp.setCANCTRL_Mode(MODE_NORMAL) == MCP2515_OK) { //MODE_NORMAL MODE_LOOPBACK
        //     printf("OK\n\r");
    } else {
        printf("failed\n\r");
        return -1;
    }

    _mcp.dumpExtendedStatus();
    wait(.001);

    if (res != MCP2515_OK) {
        return 0;

    }
    return 1;
}