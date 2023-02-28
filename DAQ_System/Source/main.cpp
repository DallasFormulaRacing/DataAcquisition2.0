// Code from: https://os.mbed.com/users/hudakz/code/SDCard//file/d08c053d5a35/main.cpp/

/* Relevant files
*   SDBlockDevice not found: https://os.mbed.com/questions/82585/SDBlockDeviceh-not-found/
*   MCU Pinout diagram: https://os.mbed.com/platforms/ST-Nucleo-F446RE/
*   SD Pinout: https://learn.adafruit.com/adafruit-microsd-spi-sdio/pinouts
*   Corruption: https://forums.mbed.com/t/os-6-sd-card-filesystem-gets-corrupted-when-written-to/16632/2
*   Properly Saving data: https://os.mbed.com/questions/69263/Saving-analog-data-in-SD-card/
*/


/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "File.h"
#include "mbed.h"
#include <cstdint>
#include <stdio.h>
#include <cstdio>
#include <errno.h>
#include <time.h>

#include "SDBlockDevice.h"
#include "FATFileSystem.h"

#define kBlockSectorByteSize 512

int Mount(FATFileSystem*, SDBlockDevice*);
int Unmount(FATFileSystem*);
FILE* FileOpen(const char*);
int FileClose(FILE*);
int FileWrite(FILE*, const char*);

// Entry point for the example
int main() {
    // Physical block device, can be any device that supports the BlockDevice API
    SDBlockDevice   block_device(PA_7, PA_6, PA_5, PB_6);  // mosi, miso, sck, cs

    // File system declaration
    FATFileSystem   file_system("fs");
    
    // buffer string used to write to a file
    char buffer[kBlockSectorByteSize] = "\0"; 

    // used for error checking when operating the block device
    int status = 0;

    // sensor values
    int linpot1 = 0;
    int linpot2 = 0;
    int linpot3 = 0;
    int linpot4 = 0;

    printf("--- CSV File Test ---\n");

    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);
    Mount(&file_system, &block_device);

    // Open the numbers file
    printf("Opening \"/fs/data.csv\"... ");
    fflush(stdout);

    FILE* data_file = FileOpen("/fs/data.csv");

    // write the first row to the file with some arbitrary sensor names
    FileWrite(data_file, "Time (sec), LinPot1 (in/s), LinPot2 (in/s), LinPot3 (in/s), LinPot4 (in/s)\n");

    // fill the file with arbitrary numbers (this is just a proof of concept, these are intentionally bs)
    clock_t timer = clock();
    for(int i = 0; i < 100; i++) {
        snprintf(buffer, sizeof(buffer), "%f, %d, %d, %d, %d\n", ((float)timer)/CLOCKS_PER_SEC, linpot1, linpot2, linpot3, linpot4);
        FileWrite(data_file, buffer);
        linpot1++;
        linpot2++;
        linpot3++;
        linpot4++;
        timer = clock() - timer;
    }

    // Close the file which also flushes any cached writes
    printf("Closing \"/fs/data.csv\"... ");
    fflush(stdout);
    FileClose(data_file);

    // Tidy up
    printf("Unmounting... ");
    fflush(stdout);
    Unmount(&file_system);
    
    printf("\r\n");
    
    printf("CSV File Test Done!\n");
}

// mounts the filesystem to the given sd block device and checks for errors, reformats the device of no filesystem is found
int Mount(FATFileSystem *file_system, SDBlockDevice *block_device) {
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
int Unmount(FATFileSystem *file_system) {
    int status = file_system->unmount();
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    return status;
}

// opens the file at the given file path and creates it if it doesn't exist
FILE* FileOpen(const char* file_path) {
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
int FileClose(FILE* file) {
    int status = fclose(file);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

// writes the given input to the given file, to allow a specified format, we use sprintf() to print the format to a buffer string and then send the buffer to file_write()
int FileWrite(FILE* file, const char* input) {
    int status = fprintf(file, input);
    if (status < 0) {
        printf("Fail :(\n");
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
    return status;
}

/*
TODO:
File read function
Open, read, and close file directory functions
*/
