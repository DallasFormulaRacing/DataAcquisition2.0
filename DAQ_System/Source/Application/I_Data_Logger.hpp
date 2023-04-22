/*
* SD Data Logger
* Authors:        Cristian Cruz
*                 Ethan Fischer
*
* Email:          cris14.alex@gmail.com
*                 ejf180001@utdallas.edu
* 
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IDATA_LOGGER
#define IDATA_LOGGER

#include "BlockDevice.h"
#include "mbed.h"

namespace application {
    
class I_Data_Logger {
    public:
        virtual ~I_Data_Logger() = default;

        virtual uint8_t Mount(BlockDevice*) = 0;
        virtual uint8_t Unmount() = 0;
        virtual uint8_t FileOpen(char*) = 0;
        virtual uint8_t FileClose() = 0;
        virtual uint8_t FileWrite(const char*) = 0;

        char write_buffer_;
};

}

#endif // IDATA_LOGGER