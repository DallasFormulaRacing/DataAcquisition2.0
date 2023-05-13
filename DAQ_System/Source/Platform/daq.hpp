/*
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef DAQ_H
#define DAQ_H

// C/C++ Standard Libraries
#include <memory>

// DFR Custom Dependancies
#include "Adapter/Interfaces/ilinear_potentiometer.hpp"
#include "Application/I_Data_Logger.hpp"

namespace platform {

// (4 linpot integers) * (4 bytes allocated per integer) = 16, 4 commas = 4 chars = 4 bytes, 1 more char & byte for the newline, 16 + 4 + 1 = 21
// #define BUFFER_SIZE 21

// (4 linpot floats + 1 timestamps float) * (4 bytes allocated per float) +
// (4 commas + 1 newline) * (1 byte allocated per char) +
#define BUFFER_SIZE 25

class DAQ {
    public:
        DAQ();
        ~DAQ();

        void Init();
        void Read();
        void Write(float timestamp, bool data_logging_enable);

    // private:
        struct SuspensionPotentiometers {
            std::unique_ptr<adapter::ILinear_Potentiometer> front_left;
            std::unique_ptr<adapter::ILinear_Potentiometer> front_right;
            std::unique_ptr<adapter::ILinear_Potentiometer> rear_left;
            std::unique_ptr<adapter::ILinear_Potentiometer> rear_right;
        };

        SuspensionPotentiometers suspension_pots;
        std::unique_ptr<application::I_Data_Logger> data_logger;


        uint8_t open_file = 0;
        // used for opening a file
        char file_name[16] = "\0";
        uint8_t file_name_status = 0;
        char write_buffer[BUFFER_SIZE] = "\0";
        uint8_t status = 0;

};


}

#endif // DAQ_H