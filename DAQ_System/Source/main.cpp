#include "mbed.h"
#include <cstdio>
#include <cstdlib>


#include "L3GD20H.h"
// Found in: https://os.mbed.com/users/julioefajardo/code/L3GD20H/docs/tip/classL3GD20H.html


constexpr float k250DPSSensitivity = 0.00875f;// this is using a sensitivity of 250 dps which is equivalent to .00875
constexpr float kDegreesToRadians = (3.14159f/180); // this is the conversion factor for degrees per second to radians per second all you have to do is multiply the callibrated data or the raw data by the conversion factor
L3GD20H gyro(I2C_SDA, I2C_SCL);

void AverageOffsetCalc(short offset[], short sum[]);
void DegreesPerSecondConversion(short degrees_per_second[],short offset[], short gyro_data[]);
void RadiansPerSecondConversion(float radians_per_second[],short degrees_per_second[]);

int main(){

    
    short gyro_data[3] = {0, 0, 0};

    // for the 3 arrays below the first element will deal with purely the x-axis the second element is y and the last element is z. These will be used to calculate an average
    // that will be used for the offeset for the callibration of the gyroscope. Int n is used to control the sample size for the average.
    short sum[3] = {0}; 
    short gyro_offset[3] = {0}; 
    short degrees_per_second[3] = {0};
    float radians_per_second[3] = {0};


    // logical conditions to determine the branches that will exeute in the code
    bool gyro_status;
    bool offsetStatus = false;
    
 
    while (true){
 
        gyro_status = gyro.read(gyro_data);

        if(gyro_status == false){
               printf( "It's not Gyroin Time =( \r");
               
        }
        else{
            /* the if statement is used to find the offset needed to zero the gyroscope in steady state. This is done by finding and average of the values in the 
            sample size of n when stationary. this only needs to be done once so this if statement wont really every repeat.
             */
            if(offsetStatus != true){
                AverageOffsetCalc(gyro_offset, sum);
                offsetStatus = true; // offset is set so this setting makes it so it wont callibrate every single time.
            }
                

            DegreesPerSecondConversion(degrees_per_second, gyro_offset, gyro_data);
            RadiansPerSecondConversion(radians_per_second, degrees_per_second);

            /* this is the ANSI escape sequences 
            \033 is the escape character, which signals the start of an escape sequence. 
            [2J is the clear screen command, which clears the entire screen.
            [1;1H is the cursor position command, which moves the cursor to a specified     
            */
                   
            //raw data from the gyroscope
            //printf("\033[2J\033[1;1H"); 
            // printf("tx: %d \n",gyro_data[0] ); 
            //printf("ty: %d \n",gyro_data[1] );
            //printf("tz: %d \n",gyro_data[2] );
            // displays the average offset used for callibration of the gyroscope
            //printf("the average of the X-axis is: %d \n", average_offset[0]);
            //printf("the average of the y-axis is: %d \n", average_offset[1]);
            //printf("the average of the z-axis is: %d \n", average_offset[2]);
            // displays the callibrated data of the gyroscope in dps
            //printf("callibration of X(in dps): %d \n",degrees_per_second[0]);
            //printf("callibration of Y(in dps): %d \n",degrees_per_second[1]);
            //printf("callibration of Z(in dps): %d \n",degrees_per_second[2]);
            // displays the callibrated data of the gyroscope in radians per second
            //printf("callibration of x(in rad/s) %.5f \n",radians_per_second[0]);
            //printf("callibration of y(in rad/s) %.5f \n",radians_per_second[1]);
            //printf("callibration of z(in rad/s) %.5f \n",radians_per_second[2]);

            //thread_sleep_for(500);
        } 
    }             
}


void AverageOffsetCalc(short Offset[], short sum[]){
  // collecting the sampled data for the offset average
  short gyro_data[3] = {0};
  int average_sample_size = 15;

    for (int i = 0; i <= average_sample_size; i++){
        gyro.read(gyro_data);
        sum[0] += gyro_data[0];
        sum[1] += gyro_data[1];
        sum[2] += gyro_data[2];
    }

    Offset[0] = sum[0]/average_sample_size;
    Offset[1] = sum[1]/average_sample_size;
    Offset[2] = sum[2]/average_sample_size;

}

void DegreesPerSecondConversion(short degrees_per_second[],short offset[],short gyro_data[]){
  // the equation used to find the dps of the gyroscope is dps = (rawdata - offset)*sensitivity
    degrees_per_second[0] = (gyro_data[0] - offset[0])*k250DPSSensitivity;
    degrees_per_second[1] = (gyro_data[1] - offset[1])*k250DPSSensitivity;
    degrees_per_second[2] = (gyro_data[2] - offset[2])*k250DPSSensitivity;
}

void RadiansPerSecondConversion(float rads[],short degrees_per_second[]){
    rads[0] = kDegreesToRadians *degrees_per_second[0];
    rads[1] = kDegreesToRadians *degrees_per_second[1];
    rads[2] = kDegreesToRadians *degrees_per_second[2];
}