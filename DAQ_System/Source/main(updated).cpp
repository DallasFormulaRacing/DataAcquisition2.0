#include "mbed.h"
#include <cstdio>
#include <cstdlib>


#include "L3GD20H.h"
// Found in: https://os.mbed.com/users/julioefajardo/code/L3GD20H/docs/tip/classL3GD20H.html


#define sensitivity (.00875 )// this is using a sensitivity of 250 dps which is equivalent to .00875
#define DPS_TO_RadS (3.1459/180) // this is the conversion factor for degrees per second to radians per second all you have to do is multiply the callibrated data or the raw data by the conversion factor



// main() runs in its own thread in the OS
int main()
{
    L3GD20H Gyro(I2C_SDA, I2C_SCL);

    short gyroData[3] = {0, 0, 0};
    // for the 3 arrays below the first element will deal with purely the x-axis the second element is y and the last element is z. These will be used to calculate an average
    // that will be used for the offeset for the callibration of the gyroscope. Int n is used to control the sample size for the average.
    short sum[3] = {0}; 
    short average[3] = {0}; 
    short callibrated[3] = {0};
    float rads[3] = {0};
    int n = 15;
    bool offsetStatus = false;

    

    // The for loop serves as a never ending loop 
    for(;;)
    {

            while (Gyro.read(gyroData)) 
            {

        

                /* the if statement is used to find the offset needed to zero the gyroscope in steady state. This is done by finding and average of the values in the 
                    sample size of n when stationary. this only needs to be done once so this if statement wont really every repeat.
                */
                if(offsetStatus != true)
                {
                    // collecting the sampled data for the offset average
                    for (int i = 0; i <= n; i++) 
                    {
                        Gyro.read(gyroData);
                        sum[0] += gyroData[0];
                        sum[1] += gyroData[1];
                        sum[2] += gyroData[2];
                        
                        // the if statment is used to find the average and will be calculated when all sample data is retrieved
                        if( i == n )
                        {
                            average[0] = sum[0]/n;
                            average[1] = sum[1]/n;
                            average[2] = sum[2]/n;
                        }
                    }


                    offsetStatus = true; // offset is set so this setting makes it so it wont callibrate every single time.
                }

                // the equation used to find the dps of the gyroscope is dps = (rawdata - offset)*sensitivity
                callibrated[0] = (gyroData[0] - average[0])*sensitivity;
                callibrated[1] = (gyroData[1] - average[1])*sensitivity;
                callibrated[2] = (gyroData[2] - average[2])*sensitivity;
                // this is the conversion to radians per second
                rads[0] = DPS_TO_RadS *callibrated[0];
                rads[1] = DPS_TO_RadS *callibrated[1];
                rads[2] = DPS_TO_RadS *callibrated[2];
              
                

            

                /* this is the ANSI escape sequences 
             \033 is the escape character, which signals the start of an escape sequence. 
            [2J is the clear screen command, which clears the entire screen.
            [1;1H is the cursor position command, which moves the cursor to a specified 
            
               */
                // raw data from the gyroscope
                printf("\033[2J\033[1;1H"); 
                printf("tx: %d \n",gyroData[0] ); 
                printf("ty: %d \n",gyroData[1] );
                printf("tz: %d \n",gyroData[2] );
                // displays the average offset used for callibration of the gyroscope
                printf("the average of the X-axis is: %d \n", average[0]);
                printf("the average of the y-axis is: %d \n", average[1]);
                printf("the average of the z-axis is: %d \n", average[2]);
                // displays the callibrated data of the gyroscope in dps
                printf("callibration of X(in dps): %d \n",callibrated[0]);
                printf("callibration of Y(in dps): %d \n",callibrated[1]);
                printf("callibration of Z(in dps): %d \n",callibrated[2]);
                // displays the callibrated data of the gyroscope in radians per second
                printf("callibration of x(in rad/s) %.5f \n",rads[0]);
                printf("callibration of y(in rad/s) %.5f \n",rads[1]);
                printf("callibration of z(in rad/s) %.5f \n",rads[2]);
                // this is still under testing 
                

                thread_sleep_for(500);

            

                
            }
        
            while (!Gyro.read(gyroData)) // when the gyroscope fails to read data then the this part of the code will execute until a successfull read is detected.
            {
                printf( "It's not Gyroin Time =( \r");

            }
    
    }
}