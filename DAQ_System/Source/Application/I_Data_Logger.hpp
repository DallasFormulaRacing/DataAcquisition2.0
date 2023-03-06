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

#include "mbed.h"

namespace application {
    
class I_Data_Logger {
    public:
        virtual ~I_Data_Logger() = default;

        virtual uint8_t Mount(FileSystem*, BlockDevice*) = 0;
        virtual uint8_t Unmount(FileSystem*) = 0;
        virtual FILE* FileOpen(const char*) = 0;
        virtual uint8_t FileClose(FILE*) = 0;
        virtual uint8_t FileWrite(FILE*, const char*) = 0;
        virtual const char* NewLogSessionFile() = 0;

        char write_buffer;
        FILE* data_file;

};

}

#endif // IDATA_LOGGER