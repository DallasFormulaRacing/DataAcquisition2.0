#include "mbed.h"
#include "Adapters/Accelerometer/accelerometer_lsm303dlhc.h"
#include <iostream>


int main() {
    Accelerometer_LSM303DLHC accelerometer(I2C_SDA, I2C_SCL);
    accelerometer.init();    
    float* acceleration = nullptr;

    std::cout << "===========New Test===========" << std::endl;
    while (true) {
        accelerometer.ComputeAcceleration();
        acceleration = accelerometer.GetAcceleration();

        //accelerometer values must be 
        std::cout << "ACC:" << 
                     "\tX: " << acceleration[0] <<
                     "\tY: " << acceleration[1] <<
                     "\tZ: " << acceleration[2] << std::endl;
        
        ThisThread::sleep_for(500ms);
    }
}
