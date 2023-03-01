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

#include "data_logger.hpp"
#include "FATFileSystem.h"

namespace adapter {

// buffer string used to write to a file
// kBlockSectorByteSize is defined in data_logger.hpp
char write_buffer[kBlockSectorByteSize] = "\0";

DataLogger::DataLogger(PinName mosi, PinName miso, PinName sck, PinName cs) {
    block_device = new SDBlockDevice(mosi, miso, sck, cs);
    file_system = new FATFileSystem("fs");

    Mount(file_system, block_device);
    
    data_file = FileOpen("/fs/data.csv");
}

DataLogger::~DataLogger() {
    FileClose(data_file);
    Unmount(file_system);
}

// mounts the filesystem to the given sd block device and checks for errors, reformats the device of no filesystem is found
int DataLogger::Mount(FileSystem *file_system, BlockDevice *block_device) {
    int status = file_system->mount(block_device);
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
int DataLogger::Unmount(FileSystem *file_system) {
    int status = file_system->unmount();
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    return status;
}

// opens the file at the given file path and creates it if it doesn't exist
FILE* DataLogger::FileOpen(const char* file_path) {
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
int DataLogger::FileClose(FILE* file) {
    int status = fclose(file);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

// writes the given input to the given file, to allow a specified format, we use sprintf() to print the format to a write_buffer string and then send the write_buffer to file_write()
int DataLogger::FileWrite(FILE* file, const char* input) {
    int status = fprintf(file, input);
    if (status < 0) {
        printf("Fail :(\n");
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

}