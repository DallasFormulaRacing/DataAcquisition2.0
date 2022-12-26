#include "mbed.h"
#include "LSM303DLHC.h"
#include <iostream>

// main() runs in its own thread in the OS
int main() {

    LSM303DLHC sensor_obj(I2C_SDA, I2C_SCL);
    sensor_obj.init();
    // accel_obj.frequency(75);

    int acc_data[3] = {0, 0, 0};
    int mag_data[3] = {0, 0, 0};

    std::cout << "===========New Test" << std::endl;
    ThisThread::sleep_for(500ms);
    

    while (true) {

        sensor_obj.read(acc_data, mag_data);
        ThisThread::sleep_for(500ms);

        std::cout << "ACC:" << 
                     "\tX: " << acc_data[0] <<
                     "\tY: " << acc_data[1] <<
                     "\tZ: " << acc_data[2] << std::endl;

        std::cout << "MAG:" << 
                     "\tX: " << mag_data[0] <<
                     "\tY: " << mag_data[1] <<
                     "\tZ: " << mag_data[2] << std::endl;
        
        std::cout << std::endl;
        ThisThread::sleep_for(500ms);

    }
}

