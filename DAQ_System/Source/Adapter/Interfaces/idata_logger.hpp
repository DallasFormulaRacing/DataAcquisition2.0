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

namespace adapter {
    
class IData_Logger {
    public:
        virtual ~IData_Logger() = default;

        virtual int Mount(FileSystem*, BlockDevice*) = 0;
        virtual int Unmount(FileSystem*) = 0;
        virtual FILE* FileOpen(const char*) = 0;
        virtual int FileClose(FILE*) = 0;
        virtual int FileWrite(FILE*, const char*) = 0;

};

}

#endif // IDATA_LOGGER