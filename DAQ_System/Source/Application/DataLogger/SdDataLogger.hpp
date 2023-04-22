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

namespace application {

class SdDataLogger : public application::I_Data_Logger {
    public:
        SdDataLogger(PinName mosi, PinName miso, PinName sck, PinName cs);
        virtual ~SdDataLogger();
        
        virtual uint8_t Mount(BlockDevice*) override;
        virtual uint8_t Unmount() override;
        virtual uint8_t FileOpen(FILE**, char*) override;
        virtual uint8_t FileClose(FILE*) override;
        virtual uint8_t FileWrite(FILE*, const char*) override;

        static constexpr int kBlockSectorByteSize = 512;
        char write_buffer_[kBlockSectorByteSize]; // write buffer
        
    
    private:
        SDBlockDevice block_device_; // physical block device
        FATFileSystem file_system_{"fs"}; // file system
        char file_name_[16];
};
    
}

#endif // DATA_LOGGER_H
