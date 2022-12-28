/******************************************************************************
 *
 * Controller Area Network (CAN) Demo-Application
 * Atmel AVR with Microchip MCP2515
 *
 * Copyright (C) 2005 Martin THOMAS, Kaiserslautern, Germany
 * <eversmith@heizung-thomas.de>
 * http://www.siwawi.arubi.uni-kl.de/avr_projects
 *
 *****************************************************************************
 *
 * File    : mcp2515.c
 * Version : 0.9
 *
 * Summary : MCP2515 "low-level" driver
 *
 * Parts of this code are adapted from a MCP2510 sample-application
 * by KVASER AB, http://www.kvaser.com (KVASER-code is marked as free)
 *
 * This code-module is free to use but you have to keep the copyright
 * notice.
 *
 *
 *****************************************************************************
 *
 * File    : mcp2515.cpp (mbed LPC1768 version)
 * Version : 0.1
 *
 * All credits to the nerds above, this source has been adapted for the
 * LPC1768 platform by J.Engelman. And doesn't require and of the copyrighted
 * SPI or AVR controller code that Martin or co have excluded copyright.
 * This module remains free.
 *
 *
 *****************************************************************************/

#include "mcp2515.h"

#include "mbed.h"
#include "mcp2515_can.h"
#include "mcp2515_defs.h"
#include "mcp2515_bittime.h"

#define SPI_NULL (0x00)

#define PHSEG11        4
#define PHSEG21        1
#define BTLMODE2        7
//BTLMODE 0x80 (128)
#define BRP2        2
#define BRP1        1
#define BRP0        0

#define CNF2_BTLMODE  0x80
#define CNF3          0x28
#define CNF3_SOF      0x08
#define CNF3_WAKFIL   0x04
#define CNF3_PHSEG2_MASK 0x07

mcp2515::mcp2515(SPI& _spi, PinName ncs)
        : spi(_spi), _ncs(ncs) {
    printf("\n\rmcp2515 = %d",this);
    printf("\n\rpin = %d",ncs);

}



void mcp2515::_reset() {

    _select();
    _spi_readwrite(MCP_RESET);
    _deselect();
    wait_us(1);
}

void mcp2515::setRegister(  uint8_t address,   uint8_t value) {
    _select();
    _spi_readwrite(MCP_WRITE);
    _spi_readwrite(address);
    _spi_readwrite(value);
    _deselect();
}

uint8_t mcp2515::configRate(  uint8_t canSpeed) {
    uint8_t set, cfg1, cfg2, cfg3;

    set = 0;

    switch (canSpeed) {
        case (CAN_500KBPS_8MHZ) :
            cfg1 = 0x04;
            cfg2 = 0xA0;
            cfg3 = 0x02;
        case (CAN_50KBPS_8MHZ) :
            cfg1 = 0x04; //0x09;
            cfg2 = 0xB8; //0x90;
            cfg3 = 0x05; //0x02;
        case (CAN_125KBPS) :
            cfg1 = MCP_4MHz_125kBPS_CFG1 ;
            cfg2 = MCP_4MHz_125kBPS_CFG2 ;
            cfg3 = MCP_4MHz_125kBPS_CFG3 ;
            set = 1;
            break;
        case (CAN_20KBPS) :
            cfg1 = MCP_4MHz_20kBPS_CFG1 ;
            cfg2 = MCP_4MHz_20kBPS_CFG2 ;
            cfg3 = MCP_4MHz_20kBPS_CFG3 ;
            set = 1;
            break;
        case (CAN_500KBPS_10MHz) :
            cfg1 = 0x00;
            cfg2 = 0x92;
            cfg3 = 0x02;
            set = 1;
            break;            
        default:
            set = 0;
            break;
    }

    if (set) {
        setRegister(MCP_CNF1, cfg1);
        setRegister(MCP_CNF2, cfg2);
        setRegister(MCP_CNF3, cfg3);
        return MCP2515_OK;
    } else {
        return MCP2515_FAIL;
    }
}

int mcp2515::configRate2(int bit_rate)
{
    //struct spi_device *spi = to_spi_device(can->cdev.dev);
    //struct mcp251x *chip = dev_get_drvdata(&spi->dev);
    //struct mcp251x_platform_data *pdata = spi->dev.platform_data;
        printf("\n\rcanspeed=%d",bit_rate);
    int f_osc = 20000000;
    int tqs; /* tbit/TQ */
    int brp;
    int ps1, ps2, propseg, sjw;

    /* Determine the BRP value that gives the requested bit rate. */
    for(brp = 0; brp < 8; brp++) {
        tqs = f_osc / (2 * (brp + 1)) / bit_rate;
        if (tqs >= 5 && tqs <= 25
            && (f_osc / (2 * (brp + 1)) / tqs) == bit_rate)
            break;
    }
    if (brp >= 8) printf("Spaztic BRP");
   //     return -1;

    /* The CAN bus bit time (tbit) is determined by:
     *   tbit = (SyncSeg + PropSeg + PS1 + PS2) * TQ
     * with:
     *     SyncSeg = 1
     *     sample point (between PS1 and PS2) must be at 60%-70% of the bit time
     *     PropSeg + PS1 >= PS2
     *     PropSeg + PS1 >= Tdelay
     *     PS2 > SJW
     *     1 <= PropSeg <= 8, 1 <= PS1 <=8, 2 <= PS2 <= 8
     * SJW = 1 is sufficient in most cases.
     * Tdelay is usually 1 or 2 TQ.
     */

    propseg = ps1 = ps2 = (tqs - 1) / 3;
    if (tqs - (1 + propseg + ps1 + ps2) == 2)
        ps1++;
    if (tqs - (1 + propseg + ps1 + ps2) == 1)
        ps2++;
    sjw = 1;

    printf("\n\rbit rate: BRP = %d, Tbit = %d TQ, PropSeg = %d, PS1 = %d, PS2 = %d, SJW = %d\n",
        brp, tqs, propseg, ps1, ps2, sjw);

    /* Since we can only change the bit rate when the network device is
     * down the chip must be in sleep mode. Wake it up and put it into
     * config mode. */
    //mcp251x_hw_wakeup(spi);
    //mcp251x_write_bits(spi, CANCTRL, CANCTRL_REQOP_MASK, CANCTRL_REQOP_CONF);

    //mcp251x_write_reg(spi, CNF1, ((sjw-1) << 6) | brp);
    //mcp251x_write_reg(spi, CNF2, CNF2_BTLMODE | ((ps1-1) << 3) | (propseg-1));
    //mcp251x_write_bits(spi, CNF3, CNF3_PHSEG2_MASK, (ps2-1));
        setRegister(MCP_CNF1, ((sjw-1) << 6) | brp );
        setRegister(MCP_CNF2, CNF2_BTLMODE | ((ps1-1) << 3) | (propseg-1) );
        modifyRegister(MCP_CNF3, CNF3_PHSEG2_MASK, (ps2-1) );
    //mcp251x_hw_sleep(spi);

    /* Calculate actual bit rate. */
    //chip->bit_rate = pdata->f_osc / (2 * (brp + 1)) / tqs;

    return 0;
}

uint8_t mcp2515::readRegister(  uint8_t address) {
    uint8_t ret;

    _select();
    _spi_readwrite(MCP_READ);
    _spi_readwrite(address);
    ret = _spi_read();
    _deselect();

    return ret;
}

void mcp2515::readRegisterS(  uint8_t address,
                              uint8_t values[],   uint8_t n) {
    uint8_t i;

    _select();
    _spi_readwrite(MCP_READ);
    _spi_readwrite(address);
    // mcp2515 has auto-increment of address-pointer
    for (i=0; i<n; i++) {
        values[i] = _spi_read();
    }
    _deselect();
}

void mcp2515::modifyRegister(  uint8_t address,
                               uint8_t mask,   uint8_t data) {
    _select();
    _spi_readwrite(MCP_BITMOD);
    _spi_readwrite(address);
    _spi_readwrite(mask);
    _spi_readwrite(data);
    _deselect();
}


uint8_t mcp2515::readXXStatus_helper(  uint8_t cmd) {
    uint8_t i;

    _select();
    _spi_readwrite(cmd);
    i = _spi_read();
    _deselect();

    return i;
}

uint8_t mcp2515::readStatus(void) {
    return readXXStatus_helper(MCP_READ_STATUS);
}

uint8_t mcp2515::RXStatus(void) {
    return readXXStatus_helper(MCP_RX_STATUS);
}

// read-modify-write - better: Bit Modify Instruction
uint8_t mcp2515::setCANCTRL_Mode(uint8_t newmode) {

    uint8_t i;

    i = readRegister(MCP_CANCTRL);
    i &= ~(MODE_MASK);
    i |= newmode;
    setRegister(MCP_CANCTRL, i);

    // verify as advised in datasheet
    i = readRegister(MCP_CANCTRL);
    i &= MODE_MASK;
    if ( i == newmode ) {
        return MCP2515_OK;
    } else {
        return MCP2515_FAIL;
    }
    
}


void mcp2515::setRegisterS(  uint8_t address,
                             uint8_t values[],   uint8_t n) {
    uint8_t i;

    _select();
    _spi_readwrite(MCP_WRITE);
    _spi_readwrite(address);
    // mcp2515 has auto-increment of address-pointer
    for (i=0; i<n; i++) {
        _spi_readwrite(values[i]);
    }
    _deselect();
}

void mcp2515::read_can_id(   uint8_t mcp_addr,
                             uint8_t* ext, uint32_t* can_id ) {
    uint8_t tbufdata[4];

    *ext = 0;
    *can_id = 0;

    readRegisterS( mcp_addr, tbufdata, 4 );

    *can_id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & MCP_TXB_EXIDE_M) ==  MCP_TXB_EXIDE_M ) {
        // extended id
        *can_id = (*can_id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        *can_id <<= 16;
        *can_id = *can_id +(tbufdata[MCP_EID8]<<8) + tbufdata[MCP_EID0];
        *ext = 1;
    }
}


void mcp2515::read_can_idN(   uint8_t mcp_addr,
                              CANFormat* ext, unsigned int* can_id ) {
    uint8_t tbufdata[4];

    *ext = CANStandard;
    *can_id = 0;

    readRegisterS( mcp_addr, tbufdata, 4 );

    *can_id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & MCP_TXB_EXIDE_M) ==  MCP_TXB_EXIDE_M ) {
        // extended id
        *can_id = (*can_id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        *can_id <<= 16;
        *can_id = *can_id +(tbufdata[MCP_EID8]<<8) + tbufdata[MCP_EID0];
        *ext = CANExtended;//1;
    }
}

// Buffer can be MCP_RXBUF_0 or MCP_RXBUF_1
void mcp2515::read_canMsg(   uint8_t buffer_sidh_addr, CANMessage* msg)
//CANMessage mcp2515::read_canMsg(   uint8_t buffer_sidh_addr)
{
    uint8_t mcp_addr, ctrl,dlc;



    mcp_addr = buffer_sidh_addr;

    read_can_idN( mcp_addr, &msg->format, &msg->id );

    ctrl = readRegister( mcp_addr-1 );
    dlc = readRegister( mcp_addr+4 );

    //if ((*dlc & RTR_MASK) || (ctrl & 0x08)) {
    if ((ctrl & 0x08)) {
        msg->type = CANRemote;  //1 CANRemote
    } else {
        msg->type = CANData;  //0 CANData
    }

    dlc &= MCP_DLC_MASK;
    readRegisterS( mcp_addr+5, &(msg->data[0]), dlc );
    msg->len =dlc;

}
/*
void mcp2515::setDebugConsole(Serial c){
    //console=c;
    debug=true;
} */  

void mcp2515::write_can_id(   uint8_t mcp_addr,
                              uint8_t ext,   uint32_t can_id ) {
    uint16_t canid;
    uint8_t tbufdata[4];

    canid = (uint16_t)(can_id & 0x0FFFF);

    if ( ext == 1) {
        tbufdata[MCP_EID0] = (uint8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (uint8_t) (canid / 256);
        canid = (uint16_t)( can_id / 0x10000L );
        tbufdata[MCP_SIDL] = (uint8_t) (canid & 0x03);
        tbufdata[MCP_SIDL] += (uint8_t) ((canid & 0x1C )*8);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (uint8_t) (canid / 32 );
    } else {
        tbufdata[MCP_SIDH] = (uint8_t) (canid / 8 );
        tbufdata[MCP_SIDL] = (uint8_t) ((canid & 0x07 )*32);
        tbufdata[MCP_EID0] = 0;
        tbufdata[MCP_EID8] = 0;
    }
    setRegisterS( mcp_addr, tbufdata, 4 );
}

// Buffer can be MCP_TXBUF_0 MCP_TXBUF_1 or MCP_TXBUF_2
void mcp2515::write_canMsg(   uint8_t buffer_sidh_addr,
                              CANMessage* msg) {
    uint8_t mcp_addr, dlc;

    mcp_addr = buffer_sidh_addr;
    dlc = msg->len;

    setRegisterS(mcp_addr+5, &(msg->data[0]), dlc );  // write data bytes
    write_can_id( mcp_addr, msg->format,
                  msg->id );  // write CAN id
    if ( msg->type == 1)  dlc |= MCP_RTR_MASK;  // if RTR set bit in byte
    setRegister( (mcp_addr+4), dlc );  // write the RTR and DLC
}

void mcp2515::start_transmit(  uint8_t buffer_sidh_addr) {
    // TXBnCTRL_addr = TXBnSIDH_addr - 1
    modifyRegister( buffer_sidh_addr-1 , MCP_TXB_TXREQ_M,
                    MCP_TXB_TXREQ_M );
}

uint8_t mcp2515::getNextFreeTXBuf(uint8_t *txbuf_n) {
    uint8_t res, i, ctrlval;
    uint8_t ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };

    res = MCP_ALLTXBUSY;
    *txbuf_n = 0x00;

    // check all 3 TX-Buffers
    for (i=0; i<MCP_N_TXBUFFERS; i++) {
        ctrlval = readRegister( ctrlregs[i] );
        if ( (ctrlval & MCP_TXB_TXREQ_M) == 0 ) {

            *txbuf_n = ctrlregs[i]+1; // return SIDH-address of Buffer
            res = MCP2515_OK;
            return res; /* ! function exit */
        }
    }

    return res;
}

void mcp2515::initCANBuffers(void) {
//    uint8_t i, a1, a2, a3;

    // TODO: check why this is needed to receive extended
    //   and standard frames
    // Mark all filter bits as don't care:
    write_can_id(MCP_RXM0SIDH, 0, 0);
    write_can_id(MCP_RXM1SIDH, 0, 0);
    // Anyway, set all filters to 0:
    write_can_id(MCP_RXF0SIDH, 1, 0); // RXB0: extended
    write_can_id(MCP_RXF1SIDH, 0, 0); //       AND standard
    write_can_id(MCP_RXF2SIDH, 1, 0); // RXB1: extended
    write_can_id(MCP_RXF3SIDH, 0, 0); //       AND standard
    write_can_id(MCP_RXF4SIDH, 0, 0);
    write_can_id(MCP_RXF5SIDH, 0, 0);
    /*
    // Clear, deactivate the three transmit buffers
    // TXBnCTRL -> TXBnD7
    a1 = MCP_TXB0CTRL;
    a2 = MCP_TXB1CTRL;
    a3 = MCP_TXB2CTRL;
    for (i = 0; i < 14; i++) { // in-buffer loop
        setRegister(a1, 0);
        setRegister(a2, 0);
        setRegister(a3, 0);
        a1++;
        a2++;
        a3++;
    }
    */
    // and clear, deactivate the two receive buffers.
    // setRegister(MCP_RXB0CTRL, 0);
    //setRegister(MCP_RXB1CTRL, 0);
}

uint8_t mcp2515::init(int canSpeed) {
    uint8_t res;

    _deselect();
    //MCP_CS_DDR |= ( 1 << MCP_CS_BIT );

    _reset();

    
    res = setCANCTRL_Mode(MODE_CONFIG);

    if ( res == MCP2515_FAIL ) {
        printf("\r\nCAN init failed %d\n\r",&_ncs);
        return res;  /* function exit on error */
    }
    res = configRate2(canSpeed);
    //res = configRate(CAN_500KBPS_10MHz);

    if ( res == MCP2515_OK ) {
        initCANBuffers();

        // enable both receive-buffers to receive messages
        // with std. and ext. identifiers
        // and enable rollover
        modifyRegister(MCP_RXB0CTRL,
                       MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
                       MCP_RXB_RX_STDEXT | MCP_RXB_BUKT_MASK );
        modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
                       MCP_RXB_RX_STDEXT);

        // Prescaler setting of the CLKOUT pin to zero 
        // => Spending clock frequency of the CLKOUT pin MCP2515 
        modifyRegister (MCP_CANCTRL, 0x07, CLKOUT_ENABLE);
    }

    return res;
}

/*
 * Select function
 */

void mcp2515::_select() {
//printf("{");
    _ncs = 0;
}


/*
 * Deselect function
 */

void mcp2515::_deselect() {
    _ncs = 1;
//printf("}");
}

int mcp2515::status() {
    int status = 0;
    _select();
    spi.write(0xd7);
    status = (spi.write(0x00) << 8 );
    status |= spi.write(0x00);
    _deselect();
    return status;
}

void mcp2515::_pollbusy() {
    volatile int busy = 1;
    while (busy) {
        // if bit 7 is set, we can proceed
        if ( status() & 0x80 ) {
            busy = 0;
        }
    }
}


uint8_t mcp2515::_spi_readwrite(uint8_t data) {
    //printf("W0x%x ", data);
    uint8_t ret = spi.write(data);
    //      printf("R0x%x,", ret);
    return ret;
}

uint8_t mcp2515::_spi_read(void) {
    return _spi_readwrite(SPI_NULL);
}

void mcp2515::dumpExtendedStatus(void) {
    uint8_t tec, rec, eflg;

    tec  = readRegister(MCP_TEC);
    rec  = readRegister(MCP_REC);
    eflg = readRegister(MCP_EFLG);

    printf("MCP2515 Extended Status:\n\r");
    printf("MCP Transmit Error Count %d \r\n", tec);
    printf("MCP Receiver Error Count %d \n\r", rec);
    printf("MCP Error Flag %d\n\r", eflg);

    if ( (rec>127) || (tec>127) ) {
        printf("Error-Passive or Bus-Off\n\r");
    }

    if (eflg & MCP_EFLG_RX1OVR)
        printf("Receive Buffer 1 Overflow\r\n");
    if (eflg & MCP_EFLG_RX0OVR)
        printf("Receive Buffer 0 Overflow\n\r");
    if (eflg & MCP_EFLG_TXBO)
        printf("Bus-Off\n\r");
    if (eflg & MCP_EFLG_TXEP)
        printf("Receive Error Passive\n\r");
    if (eflg & MCP_EFLG_TXWAR)
        printf("Transmit Error Warning\n\r");
    if (eflg & MCP_EFLG_RXWAR)
        printf("Receive Error Warning\r\n");
    if (eflg & MCP_EFLG_EWARN )
        printf("Receive Error Warning\n\r");
}
