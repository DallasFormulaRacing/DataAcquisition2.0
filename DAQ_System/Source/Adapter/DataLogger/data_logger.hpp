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

#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"

#include "Adapter/Interfaces/idata_logger.hpp"

#define kBlockSectorByteSize 512

namespace adapter {

class DataLogger : public adapter::IData_Logger {
    public:
        DataLogger(PinName mosi, PinName miso, PinName sck, PinName cs);
        virtual ~DataLogger();

        virtual int Mount(FileSystem*, BlockDevice*) override;
        virtual int Unmount(FileSystem*) override;
        virtual FILE* FileOpen(const char*) override;
        virtual int FileClose(FILE*) override;
        virtual int FileWrite(FILE*, const char*) override;

        SDBlockDevice* block_device; // physical block device
        FATFileSystem* file_system; // file system
        FILE* data_file; // CSV file with data
};
    
}

#endif // DATA_LOGGER_H