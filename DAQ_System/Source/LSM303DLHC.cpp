#include "mbed.h"
#include "LSM303DLHC.h"
#include "stdio.h"
 
#define MAG_ADDRESS  0x3C
#define ACC_ADDRESS  0x32
  
 
LSM303DLHC::LSM303DLHC(PinName sda, PinName scl): _device(sda, scl)
{
    _device.frequency(400000);
//    init();
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
 
void LSM303DLHC::setScale(float x, float y, float z)
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
