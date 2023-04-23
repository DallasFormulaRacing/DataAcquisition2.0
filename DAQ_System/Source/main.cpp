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

#include "SDBlockDevice.h"
#include "FATFileSystem.h"

#include "Application/DataLogger/SdDataLogger.hpp"
#include "Application/I_Data_Logger.hpp"

#define BUFFER_SIZE 128 // 128 is just an arbitrary size for now, we'll run into problems though if this ever exceeds the kBlockSectorByteSize (I think we could also just make that larger, but I'm not sure what effect that would have on writing to the file.)

using namespace application;

// Entry point for the example
int main() {

    shared_ptr<application::I_Data_Logger> data_logger = make_shared<application::SdDataLogger>(PA_7, PA_6, PA_5, PB_6); // mosi, miso, sck, cs

    // used for error checking when operating the block device
    uint8_t status = 0;

    // First Log Session:

    // sensor values
    int linpot1 = 0;
    int linpot2 = 0;
    int linpot3 = 0;
    int linpot4 = 0;

    printf("--- CSV File Test ---\n");

    // mounting the file system has moved to the constructor of the data logger class

    char file_name[16] = "\0";
    uint8_t file_name_status = 0;

    char write_buffer[BUFFER_SIZE] = "\0";

    for(int i = 0; i < 1000; i++) {
        // Increment file_name
        snprintf(file_name, sizeof(file_name), "/fs/data%d.csv", i);
        file_name_status = data_logger->FileOpen(file_name);

        if (file_name_status == 2) {
            // File not found, found a unique name
            printf("Opened %s\n", file_name);
            break;
        } else {
            // File already exists
            data_logger->FileClose();
        }
    }


    // write the first row to the file with some arbitrary sensor names
    status = data_logger->FileWrite("Time (sec), LinPot1 (in/s), LinPot2 (in/s), LinPot3 (in/s), LinPot4 (in/s)\n");

    // fill the file with arbitrary numbers (this is just a proof of concept, these are intentionally bs)
    for(int i = 0; i < 100; i++) {
        sprintf(write_buffer, "%d, %d, %d, %d, %d\n", i, linpot1, linpot2, linpot3, linpot4);
        status = data_logger->FileWrite(write_buffer);
        linpot1++;
        linpot2++;
        linpot3++;
        linpot4++;
    }

    // close the file
    status = data_logger->FileClose();
    
    // Unmounting the file system have moved to the deconstructor of the DataLogger. 
    
    printf("\r\n");
    
    printf("CSV File Test Done!\n");
}