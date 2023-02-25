#include "mbed.h"
#include "LSM303DLHC.h"
#include <iostream>

static float _lsm303Accel_MG_LSB     = 0.001F;
const int sampleCount = 500;



// main() runs in its own thread in the OS
int main() {

    LSM303DLHC sensor_obj(I2C_SDA, I2C_SCL);
    sensor_obj.init();
    
    // accel_obj.frequency(75);

    int acc_data[3] = {0, 0, 0};
    int mag_data[3] = {0, 0, 0};
    float acc_usable[3] = {0,0,0};

    std::cout << "===========New Test" << std::endl;
    ThisThread::sleep_for(500ms);
    
    double gravityRate = sensor_obj.calibrate();
    
    std::cout << "Gravity conversion = " << gravityRate << endl;
    
    while (true) {
        

        sensor_obj.read(acc_data, mag_data);
        ThisThread::sleep_for(50ms);
        
        sensor_obj.doNothing(1);
        double magnitude = sqrt(acc_data[0]*acc_data[0]+acc_data[1]*acc_data[1]+acc_data[2]*acc_data[2]);
        sensor_obj.computeAcc(acc_data, acc_usable, gravityRate);

        //accelerometer values must be 
        std::cout << "ACC:" << 
                     "\tX: " << acc_usable[0] <<
                     "\tY: " << acc_usable[1] <<
                     "\tZ: " << acc_usable[2] << std::endl;

        /*std::cout << "MAG:" << 
                     "\tX: " << mag_data[0] <<
                     "\tY: " << mag_data[1] <<
                     "\tZ: " << mag_data[2] << std::endl;*/
        
        
        //std::cout << std::endl;
        ThisThread::sleep_for(500ms);

    }
}

//testing found that ~1.028 is about 1G