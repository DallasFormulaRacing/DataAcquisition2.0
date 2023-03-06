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

namespace adapter {

// used to set a new file name for each new log session
int log_session = 0;
char file_name[16] = "\0";

// block device and file system declarations
SDBlockDevice block_device;
FATFileSystem file_system;

SdDataLogger::SdDataLogger(PinName mosi, PinName miso, PinName sck, PinName cs) : kBlockSectorByteSize(512) {
    SDBlockDevice block_device(mosi, miso, sck, cs);
    FATFileSystem file_system("fs");
    Mount(&file_system, &block_device);

    // initialize write buffer used to write to a file
    char write_buffer[kBlockSectorByteSize];
    memset(write_buffer, '\0', sizeof(write_buffer));

}

SdDataLogger::~SdDataLogger() {
    Unmount(&file_system);
}

// mounts the filesystem to the given sd block device and checks for errors, reformats the device of no filesystem is found
uint8_t SdDataLogger::Mount(FileSystem *file_system, BlockDevice *block_device) {
    uint8_t status = file_system->mount(block_device);
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        /* Reformat if we can't mount the filesystem,
           this should only happen on the first boot */
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        status = file_system->reformat(block_device);
        printf("%s\n", (status ? "Fail :(" : "OK"));
        if (status) {
            error("error: %s (%d)\n", strerror(-status), status);
        }
    }
    return status;
}

// unmounts the filesystem from the given sd block device and checks for errors
uint8_t SdDataLogger::Unmount(FileSystem *file_system) {
    uint8_t status = file_system->unmount();
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    return status;
}

// opens the file at the given file path and creates it if it doesn't exist
FILE* SdDataLogger::FileOpen(const char* file_path) {
    FILE* file = fopen(file_path, "r+");
    printf("%s\n", (!file ? "Fail :(" : "OK"));
    if(!file) {
        // create the file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        file = fopen(file_path, "w+");
        printf("%s\n", (!file ? "Fail :(" : "OK"));
        if (!file) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }
    return file;
}

// closes the given file
uint8_t SdDataLogger::FileClose(FILE* file) {
    uint8_t status = fclose(file);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

// writes the given input to the given file, to allow a specified format, we use sprintf() to print the format to a write_buffer string and then send the write_buffer to file_write()
uint8_t SdDataLogger::FileWrite(FILE* file, const char* input) {
    uint8_t status = fprintf(file, input);
    if (status < 0) {
        printf("Fail :(\n");
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

// return the file path of the new data log file for a new data log session
const char* SdDataLogger::NewLogSessionFile() {
    log_session++;
    snprintf(file_name, sizeof(file_name), "/fs/data%d.csv", log_session);
    return file_name;
}

}
