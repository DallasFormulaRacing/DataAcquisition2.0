#include "mbed.h"
#include <iostream>

#include "L3GD20H.h"
// Found in: https://os.mbed.com/users/julioefajardo/code/L3GD20H/docs/tip/classL3GD20H.html


// main() runs in its own thread in the OS
int main()
{
    L3GD20H Gyro(I2C_SDA, I2C_SCL);

    short gyroData[3] = {0, 0, 0};

    while (true) {
        Gyro.read(gyroData);
        std::cout << "It's Gyroin Time"
                  << "\tX: " << gyroData[0]
                  << "\tY: " << gyroData[1]
                  << "\tZ: " << gyroData[2] << std::endl;
        ThisThread::sleep_for(500ms);
    }
}