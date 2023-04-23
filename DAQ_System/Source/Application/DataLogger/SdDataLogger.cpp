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

#include "SdDataLogger.hpp"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"

namespace application {

SdDataLogger::SdDataLogger(PinName mosi, PinName miso, PinName sck, PinName cs) 
  : block_device_(mosi, miso, sck, cs) {
    Mount(&block_device_); // mount the file system to the defined block device
}

SdDataLogger::~SdDataLogger() {
    Unmount(); // unmount the file system from the block device
}

// mounts the filesystem to the given sd block device and checks for errors, reformats the device of no filesystem is found
uint8_t SdDataLogger::Mount(BlockDevice *block_device_) {
    printf("Mounting the filesystem... ");
    uint8_t status = file_system_.mount(block_device_);
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        /* Reformat if we can't mount the filesystem,
           this should only happen on the first boot */
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        status = file_system_.reformat(block_device_);
        printf("%s\n", (status ? "Fail :(" : "OK"));
        if (status) {
            error("error: %s (%d)\n", strerror(-status), status);
        }
    }
    return status;
}

// unmounts the filesystem from the given sd block device and checks for errors
uint8_t SdDataLogger::Unmount() {
    printf("Unmounting... ");
    uint8_t status = file_system_.unmount();
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    return status;
}

// opens the file at the given file path and creates it if it doesn't exist
// return 0 = failed to create a file
// return 1 = opened a pre-existing file
// return 2 = created a new file with a unique file name
uint8_t SdDataLogger::FileOpen(char* file_path) {
    uint8_t status = 0;
    data_file_ = fopen(file_path, "r+");

    if(!(data_file_)) {
        // create the file if it doesn't exist
        data_file_ = fopen(file_path, "w+");
        status = 2;
    }
    else {
        status = 1; // file already exists
    }

    if(!(data_file_)) {
        error("error: %s (%d)\n", strerror(errno), -errno);
        status = 0;
    }
    
    return status;
}

// closes the given file
uint8_t SdDataLogger::FileClose() {
    uint8_t status = fclose(data_file_);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

// writes the given input to the given file, to allow a specified format, we use sprintf() to print the format to a write_buffer string and then send the write_buffer to file_write()
uint8_t SdDataLogger::FileWrite(const char* input) {
    snprintf(write_buffer_, kBlockSectorByteSize, input);
    uint8_t status = fprintf(data_file_, write_buffer_);
    if (status < 0) {
        printf("Fail :(\n");
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

}
