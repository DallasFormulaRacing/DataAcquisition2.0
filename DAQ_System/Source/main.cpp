#include "mbed.h"
#include <iostream>
#include "L3GD20H.h"

// main() runs in its own thread in the OS
int main()
{
    L3GD20H Gyro(PB_7, PB_6);

    short gyroData[3] = {0, 0, 0};

    while (true) {
        Gyro.read(&gyroData);
        std::cout << "It's Gyroin Time"
                  << "\nX: " << gyroData[0]
                  << "\nY: " << gyroData[1]
                  << "\nZ: " << gyroData[2] << std::endl;
        ThisThread::sleep_for(500ms);      // Remove before submission
    }
}

