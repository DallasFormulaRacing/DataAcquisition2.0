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

#include "Adapter/DataLogger/data_logger.hpp"

using namespace adapter;

// Entry point for the example
int main() {

    DataLogger data_logger(PA_7, PA_6, PA_5, PB_6);
    
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

   // mounting the file system and opening the data file has moved to the constructor of the data logger class

    // write the first row to the file with some arbitrary sensor names
    status = data_logger.FileWrite(data_logger.data_file, "Time (sec), LinPot1 (in/s), LinPot2 (in/s), LinPot3 (in/s), LinPot4 (in/s)\n");

    // fill the file with arbitrary numbers (this is just a proof of concept, these are intentionally bs)
    clock_t timer = clock();
    for(int i = 0; i < 100; i++) {
        snprintf(buffer, sizeof(buffer), "%f, %d, %d, %d, %d\n", ((float)timer)/CLOCKS_PER_SEC, linpot1, linpot2, linpot3, linpot4);
        status = data_logger.FileWrite(data_logger.data_file, buffer);
        linpot1++;
        linpot2++;
        linpot3++;
        linpot4++;
        timer = clock() - timer;
    }

    /*
        Closing the data file and Unmounting the file system have moved to the deconstructor of the DataLogger. 
        Not sure if this is a correct/valid implementation or not though.
    */
    
    printf("\r\n");
    
    printf("CSV File Test Done!\n");
}