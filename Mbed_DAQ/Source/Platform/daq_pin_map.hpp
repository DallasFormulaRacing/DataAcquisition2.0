/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include "mbed.h"

namespace platform {

struct DaqPinMap {
    PinName analog_linpot_front_left    = PC_0;
    PinName analog_linpot_front_right   = PC_1;
    PinName analog_linpot_rear_left     = PB_0;
    PinName analog_linpot_rear_right    = PA_4;

    PinName sd_mosi = PA_7;
    PinName sd_miso = PA_6;
    PinName sd_sck  = PA_5;
    PinName sd_cs   = PB_6;
};


}