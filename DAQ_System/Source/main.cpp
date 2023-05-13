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
// #include "mbed.h"

// #include "Application/DataLogger/SdDataLogger.hpp"
// #include "Application/I_Data_Logger.hpp"

// // (4 linpot integers) * (4 bytes allocated per integer) = 16, 4 commas = 4 chars = 4 bytes, 1 more char & byte for the newline, 16 + 4 + 1 = 21
// #define BUFFER_SIZE 21

// static InterruptIn button(PC_13);
// static bool data_logging_enable = false;

// // interrupt routine activated by a falling edge of button input
// void ToggleDataLogging(void) {
//     data_logging_enable = !data_logging_enable;
// }

// int main() {
//     shared_ptr<application::I_Data_Logger> data_logger
//         = make_shared<application::SdDataLogger>(PA_7, PA_6, PA_5, PB_6); // mosi, miso, sck, cs

//     // used for error checking when operating the block device
//     uint8_t status = 0;

//     // sensor values
//     int linpot1 = 0;
//     int linpot2 = 0;
//     int linpot3 = 0;
//     int linpot4 = 0;

//     printf("--- CSV File Test ---\n");

//     // used for opening a file
//     char file_name[16] = "\0";
//     uint8_t file_name_status = 0;
//     char write_buffer[BUFFER_SIZE] = "\0";
    
//     // use internal pullup for pushbutton 
//     button.mode(PullUp);
//     // delay for initial pullup to take effect
//     wait_ns(100);
//     // attach the address of the interrupt handler routine for pushbutton
//     button.fall(&ToggleDataLogging);

//     uint8_t open_file = 0;

//     // super loop
//     printf("Entering Super Loop\n");
//     int timestamp = 0;
//     while(true) {
//         wait_ns(100);
//         // check data logger flag
//         if(data_logging_enable) {

//             if(!open_file) {
//                 // Create a unique file name
//                 for(int i = 0; i < 1000; i++) {
//                     snprintf(file_name, sizeof(file_name), "/fs/data%d.csv", i);
//                     file_name_status = data_logger->FileOpen(file_name);

//                     if (file_name_status == 2) {
//                         // File not found, found a unique name
//                         printf("Opened %s\n", file_name);
//                         break;
//                     } else {
//                         // File already exists
//                         data_logger->FileClose();
//                     }
//                 }
                
//                 open_file = 1;
//                 // write the first row to the file with some arbitrary sensor names
//                 status = data_logger->FileWrite("Time (sec), LinPot1 (in/s), LinPot2 (in/s), LinPot3 (in/s), LinPot4 (in/s)\n");
//             }

//             printf("Writing to file...\n");
//             // fill the file with arbitrary numbers (this is just a proof of concept, these are intentionally bs)
//             sprintf(write_buffer, "%d,%d,%d,%d,%d\n", timestamp, linpot1, linpot2, linpot3, linpot4);
//             status = data_logger->FileWrite(write_buffer);
//             linpot1++;
//             linpot2++;
//             linpot3++;
//             linpot4++;

//             timestamp++;

//         } else if(!data_logging_enable && open_file) {
//              // close the file
//             printf("Closing %s\n", file_name);
//             status = data_logger->FileClose();

//             // set flags
//             open_file = 0;
//         }
//     }
        
// }

//================================
/*
* DataAcquisition2.0 — 2023 Vehicle
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

// C/C++ Standard Libraries
#include <iostream>
#include <memory>

// External Dependancies
#include "mbed.h"

// DFR Custom Dependancies
#include "Platform/daq.hpp"

using AutoReloadTimer = LowPowerTicker;

static bool logging = false;
static void logging_signal() {
    logging = true;
}


int main() {
    // Init components
    platform::DAQ daq;
    daq.Init();
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3; // seconds
    AutoReloadTimer timer;
    timer.attach(&logging_signal, std::chrono::seconds(kLoggingRate));

    double timestamp = 0.0f;

    // Operate
    while (true) {
        daq.Read();

        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            daq.Write(timestamp);
            logging = false;
        }
    }
}
