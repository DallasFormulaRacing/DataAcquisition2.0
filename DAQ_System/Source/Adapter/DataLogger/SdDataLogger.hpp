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

#include "Application/I_Data_Logger.hpp"

namespace adapter {

class SdDataLogger : public application::I_Data_Logger {
    public:
        SdDataLogger(PinName mosi, PinName miso, PinName sck, PinName cs);
        virtual ~SdDataLogger();
        
        virtual uint8_t Mount(FileSystem*, BlockDevice*) override;
        virtual uint8_t Unmount(FileSystem*) override;
        virtual FILE* FileOpen(const char*) override;
        virtual uint8_t FileClose(FILE*) override;
        virtual uint8_t FileWrite(FILE*, const char*) override;
        virtual const char* NewLogSessionFile() override;

        char write_buffer; // write buffer
    
    private:
        SDBlockDevice block_device; // physical block device
        FATFileSystem file_system; // file system
        int log_session;
        char file_name[16];
        const int kBlockSectorByteSize;
};
    
}

#endif // DATA_LOGGER_H
