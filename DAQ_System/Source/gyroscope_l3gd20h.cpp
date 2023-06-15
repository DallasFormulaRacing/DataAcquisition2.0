
#include "gyroscope_l3gd20h.hpp"
#include "L3GD20H.h"
#include "mbed.h"

#define GYR_ADDRESS 0xD6


namespace adapter{

Gyroscope_L3GD20H::Gyroscope_L3GD20H(PinName sda, PinName scl)
    : _gyroscope(sda, scl){
    init(sda,scl);
}

Gyroscope_L3GD20H::~Gyroscope_L3GD20H(){}

void Gyroscope_L3GD20H::init(PinName sda, PinName scl){

    char reg_v;
    _gyroscope.get_L3GD20H_().frequency(100000);

    // Normal power mode, all axes enabled
    reg_v = 0;
    reg_v |= 0x0F;
    _gyroscope.writeRegister(GYR_ADDRESS, L3GD20_CTRL_REG1, reg_v);
}


void Gyroscope_L3GD20H::GyroscopeOffset(short Offset_average[3]){
    short gyroscopeL3GD20Hdata[3] = {0}, sum[3] = {0};
    int averageSampleSize = 15;

    for (int i = 0; i <= averageSampleSize; i++){
        _gyroscope.read(gyroscopeL3GD20Hdata);
        sum[0] += gyroscopeL3GD20Hdata[0];
        sum[1] += gyroscopeL3GD20Hdata[1];
        sum[2] += gyroscopeL3GD20Hdata[2];
    }

    Offset_average[0] = sum[0]/averageSampleSize;
    Offset_average[1] = sum[1]/averageSampleSize;
    Offset_average[2] = sum[2]/averageSampleSize;
}

bool Gyroscope_L3GD20H::ComputeDegreesPerSecond(short L3GD20HDataArray[3],short Offset_average[3]){

    
    short GyroscopeL3GD20HRawData[3];
    if(_gyroscope.read(GyroscopeL3GD20HRawData)){ // first element is the x-axis, second is the y-axis, and the third element is the z-axis
        
        // the next three equations are the conversion of raw data to degrees/second. Equation used is degrees/second = RawData * SensitivityOfFullScalRange. default sensitivity for L3GD20H is 250dps
        L3GD20HDataArray[0] =  (GyroscopeL3GD20HRawData[0] - Offset_average[0] ) * DPS250Sensitivity;
        L3GD20HDataArray[1] =  (GyroscopeL3GD20HRawData[1] - Offset_average[1]) * DPS250Sensitivity;
        L3GD20HDataArray[2] =  (GyroscopeL3GD20HRawData[2] - Offset_average[2]) * DPS250Sensitivity;

        return true;
    }

    else
        return false;
}

bool Gyroscope_L3GD20H::ComputeRadiansPerSecond(float L3GD20HDataArray[3], short Offset_average[3]){
    
    short L3GD20HTempData[3]; // array used for storing intermediary L3GD20H data

    if(_gyroscope.read(L3GD20HTempData)){
        ComputeDegreesPerSecond(L3GD20HTempData,Offset_average); // Easier to first obtain the degrees/second array then convert it to radians/second

        // conversion of Degrees/second to radians/second using a conversion facotr of (2pi/360)
        L3GD20HDataArray[0] = L3GD20HTempData[0] * DegreesToRadians;
        L3GD20HDataArray[1] = L3GD20HTempData[1] * DegreesToRadians;
        L3GD20HDataArray[2] = L3GD20HTempData[2] * DegreesToRadians;

        return true;
    }
    else 
        return false;
}
}