
#include "l3gd20h.hpp"
#include <stdint.h>

#include "i2c.h"
extern I2C_HandleTypeDef hi2c1;



Gyroscope_L3GD20H::Gyroscope_L3GD20H(){

    WriteReg(GYR_ADDRESS, L3GD20_CTRL_REG1, ctrl_reg1_init_value);
    ComputeInitialOffset();
}
/**
  * @brief  Obtains the raw data from gyroscope sensor.
  * @param  arr array that will hold the x,y, and z axis data from the gyroscope
  * @retval status of function.
  */

bool Gyroscope_L3GD20H::ReadRawData(short arr[3]) {
    uint8_t bytes[6] = {0};

    if (Receive(bytes)) {
    //scale is 1 dps/digit
        arr[0] = (bytes[1] << 8 | bytes[0]);
        arr[1] = (bytes[3] << 8 | bytes[2]);
        arr[2] = (bytes[5] << 8 | bytes[4]);

        return true;
    }

    return false;
}

/**
  * @brief  obtains angular velocity of the gyroscope with reference point being the stationary position
  * the gyroscope started its calibration in.
  * @retval status of function
  */
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

/**
  * @brief  obtains degrees/second from gyroscope
  * @retval pointer to the degrees_per_second values
  */
short* Gyroscope_L3GD20H::GetDegreesPerSecond() {
    // Equation: degrees/second = RawData * SensitivityOfFullScaleRange
    // Using a default sensitivity for L3GD20H is 250dps
	ComputeAngularVelocity();
    degrees_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity;
    degrees_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity;
    degrees_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity;

    return degrees_per_second_;
}

/**
  * @brief  obtains radians/second from gyroscope.
  * @retval pointer to the radians_per_second values
  */
float* Gyroscope_L3GD20H::GetRadiansPerSecond() {
	ComputeAngularVelocity();
    radians_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity * DegreesToRadians;

    return radians_per_second_;
}

/**
  * @brief  obtains an an average used to calibrate the gyroscope
  * @retval None
  */
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

/**
  * @brief  writes selected value to a selected register in the gyroscope via I2C
  * @param  addr_i2c is the slave address of the gyroscope
  * @param  addr_reg is the selected register address to write to
  * @param  register_data is the selected data to write to the selected register
  * @retval status of function
  */
bool Gyroscope_L3GD20H::WriteReg(uint8_t addr_i2c, uint8_t addr_reg, uint8_t register_data){
    uint8_t data[2] = {addr_reg, register_data};
    return HAL_I2C_Master_Transmit(&hi2c1,addr_i2c, data,2, HAL_MAX_DELAY);
}

/**
  * @brief  reads values from the selected register
  * @param  addr_i2c is the slave address of the gyroscope
  * @param  addr_reg is the selected register address to read from
  * @param  register_data is array for storing the read content of the selected register
  * @retval status of function
  */
bool Gyroscope_L3GD20H::ReadReg(uint8_t addr_i2c,uint8_t *addr_reg, uint8_t read_data[]){
	if(HAL_I2C_Master_Transmit(&hi2c1, addr_i2c, addr_reg, 1, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}
	if(HAL_I2C_Master_Receive(&hi2c1, addr_i2c, read_data, 1, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}
	return true;
}

/**
  * @brief  sequential reading function for l3g20h. Reads all the registers that contain axis
  * data. [0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D]
  * @param  data_buf array to contain all the raw axis data from l3gd20h
  * @retval status of function
  */
bool Gyroscope_L3GD20H::Receive(uint8_t data_buf[]){
	uint8_t addr_reg = L3GD20_OUT_X_L | 0x80; // to set sequential reading you need to make the MSB of the register address to 1;
	if(HAL_I2C_Master_Transmit(&hi2c1, GYR_ADDRESS, &addr_reg, 1, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}
	if(HAL_I2C_Master_Receive(&hi2c1, GYR_ADDRESS, data_buf, 6, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}

	return true;
}


