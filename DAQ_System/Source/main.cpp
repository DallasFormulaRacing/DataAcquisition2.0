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
#include "Adapter/Interfaces/ilinear_potentiometer.hpp"

// This file should not use hardware-dependent includes
// TODO: A component that serves component assembly using abstract classes

int main() {
    // Init
    shared_ptr<adapter::ILinear_Potentiometer> linear_potentiometer; // todo, get concrete class

    // Operate
    while (true) {
        linear_potentiometer->ComputeDisplacementPercentage();
        double displacement_percentage = linear_potentiometer->GetDisplacementPercentage();
        std::cout << displacement_percentage << "%" << std::endl;
    }

}
