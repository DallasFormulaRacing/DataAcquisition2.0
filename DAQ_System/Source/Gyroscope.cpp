#include "mbed.h"
#include <cstdio>


// Found in: https://os.mbed.com/users/julioefajardo/code/L3GD20H/docs/tip/classL3GD20H.html

#include "gyroscope_l3gd20h.hpp"

adapter::IGyroscope* gyro = new adapter::Gyroscope_L3GD20H(I2C_SDA,I2C_SCL);

int main(){
    // for the 3 arrays below the first element will deal with purely the x-axis the second element is y and the last element is z. These will be used to calculate an average
    // that will be used for the offeset for the callibration of the gyroscope. Int n is used to control the sample size for the average.
    short gyro_offset[3] = {0}; 
    short degrees_per_second[3] = {0};
    float radians_per_second[3] = {0};
    // logical conditions to determine the branches that will exeute in the code
    bool offsetStatus = false;
    
    while (true){
        if( gyro->ComputeDegreesPerSecond(degrees_per_second, gyro_offset) && gyro->ComputeRadiansPerSecond(radians_per_second, gyro_offset)){
            
            if(offsetStatus != true){
                gyro->GyroscopeOffset(gyro_offset);
                offsetStatus = true; // offset is set so this setting makes it so it wont callibrate every single time.
            }
            
            printf("Degrees Per Second: X = %d, Y = %d, Z = %d", degrees_per_second[0], degrees_per_second[1], degrees_per_second[2]);
            printf("   ");
            printf("Radians per second =  X = %f, Y = %f, Z = %f  \n", radians_per_second[0],radians_per_second[1], radians_per_second[2]);
        }
        else {
            printf( "It's not Gyroin Time =( \n");
        }
    }             
}



