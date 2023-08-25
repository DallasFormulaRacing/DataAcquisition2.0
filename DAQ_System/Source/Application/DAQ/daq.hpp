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
#include "Application/Interfaces/idata_logger.hpp"

namespace application {

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
            std::unique_ptr<adapter::ILinearPotentiometer> front_left;
            std::unique_ptr<adapter::ILinearPotentiometer> front_right;
            std::unique_ptr<adapter::ILinearPotentiometer> rear_left;
            std::unique_ptr<adapter::ILinearPotentiometer> rear_right;
        };

        SuspensionPotentiometers suspension_pots;
        std::unique_ptr<application::IDataLogger> data_logger;

        bool file_is_open_ = 0;
        char file_name_[16] = "\0";
        char write_buffer_[BUFFER_SIZE] = "\0";
        uint8_t status_ = 0;
};


}

#endif // DAQ_H