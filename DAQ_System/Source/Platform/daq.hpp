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

namespace platform {

class DAQ {
    public:
        DAQ();
        ~DAQ();

        void Init();
        void Read();
        void Write(double timestamp);

    // private:
        struct SuspensionPotentiometers {
            std::unique_ptr<adapter::ILinear_Potentiometer> front_left;
            std::unique_ptr<adapter::ILinear_Potentiometer> front_right;
            std::unique_ptr<adapter::ILinear_Potentiometer> rear_left;
            std::unique_ptr<adapter::ILinear_Potentiometer> rear_right;
        };

        SuspensionPotentiometers suspension_pots;

};


}

#endif // DAQ_H