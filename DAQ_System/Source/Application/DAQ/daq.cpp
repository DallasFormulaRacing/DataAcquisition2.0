/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include <iostream>

#include "daq.hpp"

// DFR Custom Dependancies
#include "Platform/component_interface_bridge.hpp"

namespace application {

DAQ::DAQ() { }

DAQ::~DAQ() { }

void DAQ::Init() {
    platform::ComponentInterfaceBridge bridge;

    suspension_pots.front_left  = bridge.GetLinearPotentiometer(platform::front_left);
    suspension_pots.front_right = bridge.GetLinearPotentiometer(platform::front_right);
    suspension_pots.rear_left   = bridge.GetLinearPotentiometer(platform::rear_left);
    suspension_pots.rear_right  = bridge.GetLinearPotentiometer(platform::rear_right);

    data_logger = bridge.GetSdDataLogger();
}

void DAQ::Read() {
    suspension_pots.front_left->ComputeDisplacementPercentage();
    suspension_pots.front_right->ComputeDisplacementPercentage();
    suspension_pots.rear_left->ComputeDisplacementPercentage();
    suspension_pots.rear_right->ComputeDisplacementPercentage();
}


void DAQ::Write(float timestamp, bool data_logging_enable) {
    if(data_logging_enable) {

        if(!open_file) {
            // Create a unique file name
            // TODO: simplify using a while loop
            for(int i = 0; i < 1000; i++) {
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
            
            open_file = 1;
            // write the first row to the file with some arbitrary sensor names
            status = data_logger->FileWrite("Time (sec), LinPot1 (in/s), LinPot2 (in/s), LinPot3 (in/s), LinPot4 (in/s)\n");
        }

        printf("Writing to file...\n");
        // fill the file with arbitrary numbers (this is just a proof of concept, these are intentionally bs)
        sprintf(write_buffer, "%f,%f,%f,%f,%f\n", timestamp,
                                                    suspension_pots.front_left->GetDisplacementInches(),
                                                    suspension_pots.front_right->GetDisplacementInches(),
                                                    suspension_pots.rear_left->GetDisplacementInches(),
                                                    suspension_pots.rear_right->GetDisplacementInches());
        
        status = data_logger->FileWrite(write_buffer);

        timestamp++;

    } else if(!data_logging_enable && open_file) {
            // close the file
        printf("Closing %s\n", file_name);
        status = data_logger->FileClose();

        // set flags
        open_file = 0;
    }
}

}
