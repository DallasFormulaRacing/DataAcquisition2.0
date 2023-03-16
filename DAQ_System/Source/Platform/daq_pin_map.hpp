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
    PinName linpot_analog_input = PC_0;
};


}