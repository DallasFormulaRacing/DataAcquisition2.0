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
#include "Adapter/LinearPotentiometer/linear_potentiometer.hpp"


using AutoReloadTimer = LowPowerTicker;

static bool sampling = false;
static void start_sampling() {
    sampling = true;
}

int main() {
    // Init
    shared_ptr<adapter::ILinear_Potentiometer> linear_potentiometer = make_shared<adapter::LinearPotentiometer>(PC_0);

    AutoReloadTimer timer;
    timer.attach(&start_sampling, 3s);

    // Operate
    while (true) {
        // Operate: Computing
        linear_potentiometer->ComputeDisplacementPercentage();

        if (sampling) {
            // Operate: Writing
            std::cout << linear_potentiometer->GetDisplacementPercentage() << "%" << std::endl;
            
            sampling = false;
        }
    }
}
