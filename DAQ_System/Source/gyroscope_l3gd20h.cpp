 /*
* Gyroscope L3GD20H
* Authors:       William Lim
*                Alex Shi    
* Modified By:   Cristian Cruz
*          
* Email:         wll180000@utdallas.edu
*                ahs190001@utdallas.edu
*                cris14.alex@gmail.com
* 
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

 #include "mbed.h"
 #include "Gyroscope_L3GD20H.hpp"


namespace adapter {

Gyroscope_L3GD20H::Gyroscope_L3GD20H(PinName sda, PinName scl)
  : _L3GD20H_(sda, scl) {
    char reg_v;
    _L3GD20H_.frequency(100000);
    
  // Normal power mode, all axes enabled and calculate average
    reg_v = 0;    
    reg_v |= 0x0F;       
    write_reg(GYR_ADDRESS,L3GD20_CTRL_REG1,reg_v);
    gyro_offset(offsetAverage);
}

bool Gyroscope_L3GD20H::read(short g[3]) {
    char gyr[6];
 
    if (recv(GYR_ADDRESS, L3GD20_OUT_X_L, gyr, 6)) {
    //scale is 1 dps/digit
        g[0] = (gyr[1] << 8 | gyr[0]);  
        g[1] = (gyr[3] << 8 | gyr[2]);
        g[2] = (gyr[5] << 8 | gyr[4]);
 
        return true;
    }
 
    return false;
}

bool Gyroscope_L3GD20H::ComputeDegreesPerSecond(short angular_velocity[3]) {

    short GyroscopeL3GD20HRawData[3];
    if(read(GyroscopeL3GD20HRawData)){ // first element is the x-axis, second is the y-axis, and the third element is the z-axis

        // the next three equations are the conversion of raw data to degrees/second. Equation used is degrees/second = RawData * SensitivityOfFullScalRange. default sensitivity for L3GD20H is 250dps
        angular_velocity[0] =  (GyroscopeL3GD20HRawData[0] - offsetAverage[0]) * DPS250Sensitivity;
        angular_velocity[1] =  (GyroscopeL3GD20HRawData[1] - offsetAverage[1]) * DPS250Sensitivity;
        angular_velocity[2] =  (GyroscopeL3GD20HRawData[2] - offsetAverage[2]) * DPS250Sensitivity;

        return true;
    }

    else
        return false;
}

bool Gyroscope_L3GD20H::ComputeRadiansPerSecond(float angular_velocity[3]){

    short L3GD20HTempData[3]; // array used for storing intermediary L3GD20H data

    if(read(L3GD20HTempData)){
        ComputeDegreesPerSecond(L3GD20HTempData); // Easier to first obtain the degrees/second array then convert it to radians/second

        // conversion of Degrees/second to radians/second using a conversion facotr of (2pi/360)
        angular_velocity[0] = L3GD20HTempData[0] * DegreesToRadians;
        angular_velocity[1] = L3GD20HTempData[1] * DegreesToRadians;
        angular_velocity[2] = L3GD20HTempData[2] * DegreesToRadians;

        return true;
    }
    else 
        return false;
}

void Gyroscope_L3GD20H::gyro_offset(short offsetAverage[3]){
    short gyroscopeL3GD20Hdata[3] = {0}, sum[3] = {0};
    int averageSampleSize = 15;

    for (int i = 0; i <= averageSampleSize; i++){
        read(gyroscopeL3GD20Hdata);
        sum[0] += gyroscopeL3GD20Hdata[0];
        sum[1] += gyroscopeL3GD20Hdata[1];
        sum[2] += gyroscopeL3GD20Hdata[2];
    }

    offsetAverage[0] = sum[0]/averageSampleSize;
    offsetAverage[1] = sum[1]/averageSampleSize;
    offsetAverage[2] = sum[2]/averageSampleSize;
    
}

bool Gyroscope_L3GD20H::write_reg(int addr_i2c,int addr_reg, char v){
    char data[2] = {static_cast<char>(addr_reg), v}; 
    return Gyroscope_L3GD20H::_L3GD20H_.write(addr_i2c, data, 2) == 0;
}

bool Gyroscope_L3GD20H::read_reg(int addr_i2c,int addr_reg, char *v){
    char data = addr_reg; 
    bool result = false;
    
    if ((_L3GD20H_.write(addr_i2c, &data, 1) == 0) && (_L3GD20H_.read(addr_i2c, &data, 1) == 0)){
        *v = data;
        result = true;
    }
    return result;
}

bool Gyroscope_L3GD20H::recv(char sad, char sub, char *buf, int length){
    if (length > 1) sub |= 0x80;
 
    return _L3GD20H_.write(sad, &sub, 1, true) == 0 && _L3GD20H_.read(sad, buf, length) == 0;
}

}