#include "mbed.h"
#include "Adapters/Accelerometer/accelerometer_lsm303dlhc.h"
#include <iostream>


int main() {

    Accelerometer_LSM303DLHC accelerometer(I2C_SDA, I2C_SCL);
    accelerometer.init();
    
    int acc_data[3] = {0, 0, 0};
    int mag_data[3] = {0, 0, 0};
    float acc_usable[3] = {0,0,0};

    std::cout << "===========New Test" << std::endl;
    ThisThread::sleep_for(500ms);
    
    double gravityRate = accelerometer.calibrate();
    
    std::cout << "Gravity conversion = " << gravityRate << endl;
    
    while (true) {
        
        accelerometer.ReadRawAcceleration();        
        accelerometer.computeAcc(acc_usable, gravityRate);

        //accelerometer values must be 
        std::cout << "ACC:" << 
                     "\tX: " << acc_usable[0] <<
                     "\tY: " << acc_usable[1] <<
                     "\tZ: " << acc_usable[2] <<
                     "\tRate: " << gravityRate << std::endl;

        /*std::cout << "MAG:" << 
                     "\tX: " << mag_data[0] <<
                     "\tY: " << mag_data[1] <<
                     "\tZ: " << mag_data[2] << std::endl;*/
        
        
        //std::cout << std::endl;
        ThisThread::sleep_for(500ms);

    }
}

//testing found that ~1.028 is about 1G