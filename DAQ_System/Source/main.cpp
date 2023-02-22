#include "mbed.h"
#include <cstdio>
#include <cstdlib>


#include "L3GD20H.h"
// Found in: https://os.mbed.com/users/julioefajardo/code/L3GD20H/docs/tip/classL3GD20H.html


#define sensitivity (.00875 )// this is using a sensitivity of 250 dps which is equivalent to .00875
#define DPS_TO_RadS (3.14159/180) // this is the conversion factor for degrees per second to radians per second all you have to do is multiply the callibrated data or the raw data by the conversion factor
L3GD20H gyro(I2C_SDA, I2C_SCL);

void averageOffsetCalc(short averageOffset[], short sum[]);
void degreesPerSecondConversion(short degreesPerSecond[],short offset[], short gyroData[]);
void radiansPerSecondConversion(float radiansPerSecond[],short degreesPerSecond[]);


// main() runs in its own thread in the OS
int main()
{

    
    short gyroData[3] = {0, 0, 0};

    // for the 3 arrays below the first element will deal with purely the x-axis the second element is y and the last element is z. These will be used to calculate an average
    // that will be used for the offeset for the callibration of the gyroscope. Int n is used to control the sample size for the average.
    short sum[3] = {0}; 
    short offset[3] = {0}; 
    short degreesPerSecond[3] = {0};
    float radiansPerSecond[3] = {0};


    // logical conditions to determine the branches that will exeute in the code
    bool gyroStatus;
    bool offsetStatus = false;
    
 
    while (true) 
    {
 
        gyroStatus = gyro.read(gyroData);

        if(gyroStatus == false)
        {
               printf( "It's not Gyroin Time =( \r");
               
        }
        else
        {
            /* the if statement is used to find the offset needed to zero the gyroscope in steady state. This is done by finding and average of the values in the 
            sample size of n when stationary. this only needs to be done once so this if statement wont really every repeat.
             */
            if(offsetStatus != true)
            {
                averageOffsetCalc(offset, sum);
                offsetStatus = true; // offset is set so this setting makes it so it wont callibrate every single time.
            }
                

            degreesPerSecondConversion(degreesPerSecond, offset, gyroData);
            radiansPerSecondConversion(radiansPerSecond, degreesPerSecond);

            /* this is the ANSI escape sequences 
            \033 is the escape character, which signals the start of an escape sequence. 
            [2J is the clear screen command, which clears the entire screen.
            [1;1H is the cursor position command, which moves the cursor to a specified     
            */
                   
            //raw data from the gyroscope
            //printf("\033[2J\033[1;1H"); 
            // printf("tx: %d \n",gyroData[0] ); 
            //printf("ty: %d \n",gyroData[1] );
            //printf("tz: %d \n",gyroData[2] );
            // displays the average offset used for callibration of the gyroscope
            //printf("the average of the X-axis is: %d \n", averageOffset[0]);
            //printf("the average of the y-axis is: %d \n", averageOffset[1]);
            //printf("the average of the z-axis is: %d \n", averageOffset[2]);
            // displays the callibrated data of the gyroscope in dps
            //printf("callibration of X(in dps): %d \n",degreesPerSecond[0]);
            //printf("callibration of Y(in dps): %d \n",degreesPerSecond[1]);
            //printf("callibration of Z(in dps): %d \n",degreesPerSecond[2]);
            // displays the callibrated data of the gyroscope in radians per second
            //printf("callibration of x(in rad/s) %.5f \n",radiansPerSecond[0]);
            //printf("callibration of y(in rad/s) %.5f \n",radiansPerSecond[1]);
            //printf("callibration of z(in rad/s) %.5f \n",radiansPersecond[2]);
            
  
                    

            //thread_sleep_for(500);

                

        } 
    }     
            
}



void averageOffsetCalc(short Offset[], short sum[])
{
  // collecting the sampled data for the offset average
  short gyroData[3] = {0};
  int averageSampleSize = 15;

    for (int i = 0; i <= averageSampleSize; i++) 
    {
        gyro.read(gyroData);
        sum[0] += gyroData[0];
        sum[1] += gyroData[1];
        sum[2] += gyroData[2];
        // the if statment is used to find the average and will be calculated when all sample data is retrieved
        if( i == averageSampleSize )
        {
            Offset[0] = sum[0]/averageSampleSize;
            Offset[1] = sum[1]/averageSampleSize;
            Offset[2] = sum[2]/averageSampleSize;
        }
    }

}

void degreesPerSecondConversion(short degreesPerSecond[],short offset[],short gyroData[])
{
            // the equation used to find the dps of the gyroscope is dps = (rawdata - offset)*sensitivity
            degreesPerSecond[0] = (gyroData[0] - offset[0])*sensitivity;
            degreesPerSecond[1] = (gyroData[1] - offset[1])*sensitivity;
            degreesPerSecond[2] = (gyroData[2] - offset[2])*sensitivity;

}

void radiansPerSecondConversion(float rads[],short callibrated[])
{
            rads[0] = DPS_TO_RadS *callibrated[0];
            rads[1] = DPS_TO_RadS *callibrated[1];
            rads[2] = DPS_TO_RadS *callibrated[2];

}
