#include "mbed.h"
#include "LSM303DLHC.h"
#include <iostream>

static float _lsm303Accel_MG_LSB     = 0.001F;
const int sampleCount = 500;
//this should be based on a calibrated value for 1.0 G
const float gravity = 1.022;

//averages data for calibration 
double averageData(double array[],int arraylength){
	double total = 0, average = 0;
	for(int k = 0; k < arraylength; k++){
		total = total + array[k];
	}
	average = total/(double)arraylength;
	return average;
}




// main() runs in its own thread in the OS
int main() {

    LSM303DLHC sensor_obj(PB_9, PB_8);
    sensor_obj.init();
    
    // accel_obj.frequency(75);

    int acc_data[3] = {0, 0, 0};
    int mag_data[3] = {0, 0, 0};

    std::cout << "===========New Test" << std::endl;
    ThisThread::sleep_for(500ms);
    
    while (true) {
        

        sensor_obj.read(acc_data, mag_data);
        ThisThread::sleep_for(50ms);

        std::cout << "ACC:" << 
                     "\tX: " << (float)acc_data[0] * _lsm303Accel_MG_LSB * gravity <<
                     "\tY: " << (float)acc_data[1] * _lsm303Accel_MG_LSB * gravity <<
                     "\tZ: " << (float)acc_data[2] * _lsm303Accel_MG_LSB * gravity << std::endl;
        
        std::cout << std::endl;
        ThisThread::sleep_for(500ms);

    }
}

//testing found that ~1.028 is about 1G