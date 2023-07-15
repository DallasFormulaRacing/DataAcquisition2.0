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
#include <cstdint>


namespace adapter {

Gyroscope_L3GD20H::Gyroscope_L3GD20H(PinName sda, PinName scl)
  : i2c_bus_(sda, scl) {
    char reg_v;
    i2c_bus_.frequency(100000);
    
    // Normal power mode, all axes enabled and calculate average
    reg_v = 0;    
    reg_v |= 0x0F;       
    WriteReg(GYR_ADDRESS, L3GD20_CTRL_REG1, reg_v);
    ComputeInitialOffset();
}

bool Gyroscope_L3GD20H::ReadRawData(short arr[3]) {
    char bytes[6] = {0};
 
    if (Receive(GYR_ADDRESS, L3GD20_OUT_X_L, bytes, 6)) {
    //scale is 1 dps/digit
        arr[0] = (bytes[1] << 8 | bytes[0]);  
        arr[1] = (bytes[3] << 8 | bytes[2]);
        arr[2] = (bytes[5] << 8 | bytes[4]);
 
        return true;
    }
 
    return false;
}

bool Gyroscope_L3GD20H::ComputeAngularVelocity() {
    short raw_data[3] = {0};
    if (ReadRawData(raw_data)) {
        angular_velocity_[0] =  raw_data[0] - offset_average_[0];
        angular_velocity_[1] =  raw_data[1] - offset_average_[1];
        angular_velocity_[2] =  raw_data[2] - offset_average_[2];

        return true;
    }

    return false;
}

short* Gyroscope_L3GD20H::GetDegreesPerSecond() {
    // Conversion of raw data to degrees/second
    // Equation: degrees/second = RawData * SensitivityOfFullScalRange
    // Using a default sensitivity for L3GD20H is 250dps
    degrees_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity;
    degrees_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity;
    degrees_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity; 

    return degrees_per_second_;
}

float* Gyroscope_L3GD20H::GetRadiansPerSecond() {
    radians_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity * DegreesToRadians; 

    return radians_per_second_;
}

void Gyroscope_L3GD20H::ComputeInitialOffset(){
    short temp_data[3] = {0};
    short sum[3] = {0};
    uint8_t average_sample_size = 15;

    for (int i = 0; i <= average_sample_size; i++) {
        ReadRawData(temp_data);
        sum[0] += temp_data[0];
        sum[1] += temp_data[1];
        sum[2] += temp_data[2];
    }

    offset_average_[0] = sum[0] / average_sample_size;
    offset_average_[1] = sum[1] / average_sample_size;
    offset_average_[2] = sum[2] / average_sample_size;
}

//=====Todo=====

bool Gyroscope_L3GD20H::WriteReg(int addr_i2c, int addr_reg, char v){
    char data[2] = {static_cast<char>(addr_reg), v}; 
    return i2c_bus_.write(addr_i2c, data, 2) == 0;
}

bool Gyroscope_L3GD20H::ReadReg(int addr_i2c,int addr_reg, char *v){
    char data = addr_reg; 
    bool result = false;
    
    if ((i2c_bus_.write(addr_i2c, &data, 1) == 0) && (i2c_bus_.read(addr_i2c, &data, 1) == 0)){
        *v = data;
        result = true;
    }
    return result;
}

bool Gyroscope_L3GD20H::Receive(char sad, char sub, char *buf, int length){
    if (length > 1) sub |= 0x80;
 
    return i2c_bus_.write(sad, &sub, 1, true) == 0 && i2c_bus_.read(sad, buf, length) == 0;
}

}