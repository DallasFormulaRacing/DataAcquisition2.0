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

#include "l3gd20h.hpp"
#include <stdint.h>


namespace sensor{

L3GD20H::L3GD20H(I2C_HandleTypeDef& hi2c):hi2c_(hi2c){
    WriteReg(GYR_ADDRESS, L3GD20_CTRL_REG1, ctrl_reg1_init_value);
    ComputeInitialOffset();
}

bool L3GD20H::ReadRawData(short arr[3]) {
    uint8_t bytes[6] = {0};

    if (Receive(bytes)) {
        arr[0] = (bytes[1] << 8 | bytes[0]);
        arr[1] = (bytes[3] << 8 | bytes[2]);
        arr[2] = (bytes[5] << 8 | bytes[4]);

        return true;
    }

    return false;
}

bool L3GD20H::ComputeAngularVelocity() {
    short raw_data[3] = {0};
    if (ReadRawData(raw_data)) {
        angular_velocity_[0] =  raw_data[0] - offset_average_[0];
        angular_velocity_[1] =  raw_data[1] - offset_average_[1];
        angular_velocity_[2] =  raw_data[2] - offset_average_[2];

        return true;
    }

    return false;
}

short* L3GD20H::GetDegreesPerSecond() {
    // Equation: degrees/second = RawData * SensitivityOfFullScaleRange
    // Using a default sensitivity for L3GD20H is 250dps
	ComputeAngularVelocity();
    degrees_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity;
    degrees_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity;
    degrees_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity;

    return degrees_per_second_;
}

float* L3GD20H::GetRadiansPerSecond() {
	ComputeAngularVelocity();
    radians_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity * DegreesToRadians;

    return radians_per_second_;
}

void L3GD20H::ComputeInitialOffset(){
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


bool L3GD20H::WriteReg(uint8_t addr_i2c, uint8_t addr_reg, uint8_t register_data){
    uint8_t data[2] = {addr_reg, register_data};
    return HAL_I2C_Master_Transmit(&hi2c1,addr_i2c, data,2, HAL_MAX_DELAY);
}



bool L3GD20H::Receive(uint8_t data_buf[]){
	uint8_t addr_reg = L3GD20_OUT_X_L | 0x80; // to set sequential reading you need to make the MSB of the register address to 1;
	if(HAL_I2C_Master_Transmit(&hi2c1, GYR_ADDRESS, &addr_reg, 1, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}
	if(HAL_I2C_Master_Receive(&hi2c1, GYR_ADDRESS, data_buf, 6, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}

	return true;
}

}
