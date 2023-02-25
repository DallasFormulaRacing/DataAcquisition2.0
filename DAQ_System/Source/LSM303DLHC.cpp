#include "mbed.h"
#include "LSM303DLHC.h"
#include "stdio.h"
#include <locale>
 
#define MAG_ADDRESS  0x3C
#define ACC_ADDRESS  0x32
  
 
LSM303DLHC::LSM303DLHC(PinName sda, PinName scl): _device(sda, scl){
    _device.frequency(400000);
}
 
void LSM303DLHC::init()
{
    // init mag
    // continuous conversion mode
    _data[0] = MR_REG_M;
    _data[1] = 0x00;
    _device.write(MAG_ADDRESS, _data, 2);
    // data rate 75hz
    _data[0] = CRA_REG_M;
    _data[1] = 0x18; // 0b00011000
    _device.write(MAG_ADDRESS, _data, 2);
    // init acc
    // data rate 100hz 
    _data[0] = CTRL_REG1_A;
    _data[1] = 0x2F; // 0b00101111
    _device.write(ACC_ADDRESS, _data, 2);
}
 
void LSM303DLHC::read(int a[3], int m[3])
{
    readAcc(a);
    readMag(m);   
}
 
void LSM303DLHC::readAcc(int a[3])
{
    _data[0] = OUT_X_L_A | (1<<7);
    _device.write(ACC_ADDRESS, _data, 1);
    _device.read(ACC_ADDRESS, _data, 6);
 
    // 12-bit values
    a[0] = (short)(_data[1]<<8 | _data[0]) >> 4;
    a[1] = (short)(_data[3]<<8 | _data[2]) >> 4;
    a[2] = (short)(_data[5]<<8 | _data[4]) >> 4;
}
 
void LSM303DLHC::readMag(int m[3])
{
    _data[0] = OUT_X_H_M;
    _device.write(MAG_ADDRESS, _data, 1);
    _device.read(MAG_ADDRESS, _data, 6);
    
    m[0] = (short) (_data[0]<<8 | _data[1]); // X
    m[1] = (short) (_data[4]<<8 | _data[5]); // Y
    m[2] = (short) (_data[2]<<8 | _data[3]); // Z
 }

void LSM303DLHC::SetScale(float x, float y, float z)
{
    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
}
 
void LSM303DLHC::setOffset(float x, float y, float z)
{
    offset[0] = x;
    offset[1] = y;
    offset[2] = z;
}

void LSM303DLHC::doNothing(int a){}

void LSM303DLHC::computeAcc(int in[3], float out[3], float gravity){
    out[0] = (float)in[0] * 0.001 / gravity;
    out[1] = (float)in[1] * 0.001 / gravity;
    out[2] = (float)in[2] * 0.001 / gravity;
}

double LSM303DLHC::calibrate()
{
    int acc_array[3] = {0,0,0};
    readAcc(acc_array);
    
    int acc_value = 0, sampleCount = 100;
    double acc_magnitude = 0, min_value = 0, max_value = 0, average_array[sampleCount],
        return_value = 0;

    //collects a number of samples to calibrate for 1G
    for(int i = 0; i < sampleCount; i++)
    {
        
        acc_magnitude = sqrt(acc_array[0] * acc_array[0] + acc_array[1] * acc_array[1] + acc_array[2] * acc_array[2]);
        average_array[i] = acc_magnitude * 0.001;
        ThisThread::sleep_for(5ms);
    }

    //finds the largest and smallest magnitudes
    min_value = average_array[0];
    if(average_array[1] <= min_value)
    {
        max_value = average_array[1];
    }
    else {
        max_value = min_value;
        min_value = average_array[1];
    }
    for(int j = 2; j < sampleCount; j++)
    {
        if(average_array[j] <= min_value)
        {
            min_value = average_array[j];
        }
        else if (average_array[j] > max_value) {
            max_value = average_array[j];
        }
    }

    /*checks that the maximum deviation is below a certain threshold. If it is too large, the conversion factor
    * will be set to a previously determined convesion factor */
    if(max_value - min_value < 0.09)
    {
        double total_array = 0;
	    for(int k = 0; k < sampleCount; k++){
		    total_array = total_array + average_array[k];
	    }
	    return_value = total_array/(double)sampleCount;
    }else {
        return_value = 1.028;
    }

    return return_value;
}