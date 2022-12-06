#ifndef MCP2515_CAN_H_
#define MCP2515_CAN_H_

#define CANDEBUG   1

#define CANUSELOOP 0

#define CANSENDTIMEOUT (200) /* milliseconds */

// initial value of gCANAutoProcess
#define CANAUTOPROCESS (1)
#define CANAUTOON  (1)
#define CANAUTOOFF (0)

#define CAN_STDID (0)
#define CAN_EXTID (1)

#define CANDEFAULTIDENT    (0x55CC)
#define CANDEFAULTIDENTEXT (CAN_EXTID)

#define CAN_20KBPS   (1)
#define CAN_125KBPS  (CAN_20KBPS+1)

#define CAN_500KBPS_10MHz (59)
#define CAN_1MBPS_8MHZ (58)
#define CAN_500KBPS_8MHZ (57)
#define CAN_250KBPS_8MHZ (56)
#define CAN_125KBPS_8MHZ (55)
#define CAN_100KBPS_8MHZ (54)
#define CAN_50KBPS_8MHZ (53)
#define CAN_20KBPS_8MHZ (52)
#define CAN_10KBPS_8MHZ (51)

#define CAN_OK         (0)
#define CAN_FAILINIT   (1)
#define CAN_FAILTX     (2)
#define CAN_MSGAVAIL   (3)
#define CAN_NOMSG      (4)
#define CAN_CTRLERROR  (5)
#define CAN_FAIL       (0xff)

#define CAN_MAX_CHAR_IN_MESSAGE (8)


#endif