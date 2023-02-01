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
#include "mbed.h"
#include <stdio.h>
#include <errno.h>

#include "SDBlockDevice.h"
#include "FATFileSystem.h"

// Physical block device, can be any device that supports the BlockDevice API
SDBlockDevice   block_device(PA_7, PA_6, PA_5, PB_6);  // mosi, miso, sck, cs

// File system declaration
FATFileSystem   file_system("fs");

// Entry point for the example
int main()
{
    printf("--- Mbed OS filesystem example ---\n");

    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);

    int status = file_system.mount(&block_device);
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        status = file_system.reformat(&block_device);
        printf("%s\n", (status ? "Fail :(" : "OK"));
        if (status) {
            error("error: %s (%d)\n", strerror(-status), status);
        }
    }

    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);

    FILE* numbers_file = fopen("/fs/numbers.txt", "r+");
    printf("%s\n", (!numbers_file ? "Fail :(" : "OK"));
    if (!numbers_file) {
        // Create the numbers file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        numbers_file = fopen("/fs/numbers.txt", "w+");
        printf("%s\n", (!numbers_file ? "Fail :(" : "OK"));
        if (!numbers_file) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }

        for (int i = 0; i < 10; i++) {
            printf("\rWriting numbers (%d/%d)... ", i, 10);
            fflush(stdout);
            status = fprintf(numbers_file, "    %d\n", i);
            if (status < 0) {
                printf("Fail :(\n");
                error("error: %s (%d)\n", strerror(errno), -errno);
            }
        }

        printf("\rWriting numbers (%d/%d)... OK\n", 10, 10);

        printf("Seeking file... ");
        fflush(stdout);
        status = fseek(numbers_file, 0, SEEK_SET);
        printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
        if (status < 0) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }

    // Go through and increment the numbers
    for (int i = 0; i < 10; i++) {
        printf("\rIncrementing numbers (%d/%d)... ", i, 10);
        fflush(stdout);

        // Get current stream position
        long pos = ftell(numbers_file);

        // Parse out the number and increment
        int32_t number;
        fscanf(numbers_file, "%ld", &number);
        number += 1;

        // Seek to beginning of number
        fseek(numbers_file, pos, SEEK_SET);

        // Store number
        fprintf(numbers_file, "    %ld\n", number);

        // Flush between write and read on same file
        fflush(numbers_file);
    }

    printf("\rIncrementing numbers (%d/%d)... OK\n", 10, 10);

    // Close the file which also flushes any cached writes
    printf("Closing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    status = fclose(numbers_file);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Display the root directory
    printf("Opening the root directory... ");
    fflush(stdout);

    DIR* root_dir = opendir("/fs/");
    printf("%s\n", (!root_dir ? "Fail :(" : "OK"));
    if (!root_dir) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    printf("root directory:\n");
    while (true) {
        struct dirent* entry = readdir(root_dir);
        if (!entry) {
            break;
        }

        printf("    %s\n", entry->d_name);
    }

    printf("Closing the root directory... ");
    fflush(stdout);
    status = closedir(root_dir);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Display the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    numbers_file = fopen("/fs/numbers.txt", "r");
    printf("%s\n", (!numbers_file ? "Fail :(" : "OK"));
    if (!numbers_file) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    printf("numbers:\n");
    while (!feof(numbers_file)) {
        int num = fgetc(numbers_file);
        printf("%c", num);
    }

    printf("\rClosing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    status = fclose(numbers_file);
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Tidy up
    printf("Unmounting... ");
    fflush(stdout);
    status = file_system.unmount();
    printf("%s\n", (status < 0 ? "Fail :(" : "OK"));
    if (status < 0) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    
    printf("Initializing the block device... ");
    fflush(stdout);

    status = block_device.init();
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        error("error: %s (%d)\n", strerror(-status), status);
    }

    printf("Erasing the block device... ");
    fflush(stdout);
    status = block_device.erase(0, block_device.size());
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        error("error: %s (%d)\n", strerror(-status), status);
    }

    printf("Deinitializing the block device... ");
    fflush(stdout);
    status = block_device.deinit();
    printf("%s\n", (status ? "Fail :(" : "OK"));
    if (status) {
        error("error: %s (%d)\n", strerror(-status), status);
    }
    
    printf("\r\n");
    
    printf("Mbed OS filesystem example done!\n");
}