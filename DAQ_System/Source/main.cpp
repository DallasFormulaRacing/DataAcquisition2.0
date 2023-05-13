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




static InterruptIn button(PC_13);
static bool data_logging_enable = false;

// interrupt routine activated by a falling edge of button input
void ToggleDataLogging(void) {
    data_logging_enable = !data_logging_enable;
}

int main() {
    // Init components
    platform::DAQ daq;
    daq.Init();
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3; // seconds
    AutoReloadTimer timer;
    timer.attach(&logging_signal, std::chrono::seconds(kLoggingRate));


    
    
    // use internal pullup for pushbutton 
    button.mode(PullUp);
    wait_ns(100);
    button.fall(&ToggleDataLogging);

    uint8_t open_file = 0;
    float timestamp = 0.0f;


    printf("Entering Super Loop\n");
    // Operate
    while (true) {
        daq.Read();

        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            daq.Write(timestamp, data_logging_enable);
            logging = false;
        }
    }
}
