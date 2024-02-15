/*
* Gyroscope L3GD20H
* Authors:       William Lim
*
* Email:         wll180000@utdallas.edu
*
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

// Standard Libraries
#include <stdint.h>

//DFR Dependencies
#include "LSM6DSOXGyroscope.hpp"

namespace sensor{

LSM6DSOX::LSM6DSOX(I2C_HandleTypeDef& hi2c):i2c_(hi2c){
    WriteReg(GYR_ADDRESS, LSM6DSOX_CTRL2_G, CTRL2_G_init_value);
    ComputeInitialOffset();
}

bool LSM6DSOX::ReadRawData(short arr[3]) {
    uint8_t bytes[6] = {0};

    if (Receive(bytes)) {
        arr[0] = (bytes[1] << 8 | bytes[0]);
        arr[1] = (bytes[3] << 8 | bytes[2]);
        arr[2] = (bytes[5] << 8 | bytes[4]);

        return true;
    }

    return false;
}

bool LSM6DSOX::AngularVelocity() {
    short raw_data[3] = {0};
    if (ReadRawData(raw_data)) {
        angular_velocity_[0] =  raw_data[0] - offset_average_[0];
        angular_velocity_[1] =  raw_data[1] - offset_average_[1];
        angular_velocity_[2] =  raw_data[2] - offset_average_[2];

        return true;
    }

    return false;
}

short* LSM6DSOX::DegreesPerSecond() {
    // Equation: degrees/second = RawData * SensitivityOfFullScaleRange
    // Using a default sensitivity for L3GD20H is 250dps
	AngularVelocity();
    degrees_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity;
    degrees_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity;
    degrees_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity;

    return degrees_per_second_;
}

float* LSM6DSOX::RadiansPerSecond() {
	AngularVelocity();
    radians_per_second_[0] = angular_velocity_[0] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[1] = angular_velocity_[1] * DPS_250_Sensitivity * DegreesToRadians;
    radians_per_second_[2] = angular_velocity_[2] * DPS_250_Sensitivity * DegreesToRadians;

    return radians_per_second_;
}

void LSM6DSOX::ComputeInitialOffset(){
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

void LSM6DSOX::SetODR(SensorConfiguration::ODR ODRValue){
		static constexpr uint8_t kNumBytes = 2;
	    uint8_t commands[kNumBytes] = {0};
	    commands[0] = LSM6DSOX_CTRL2_G;

	    // used to store the original register values
	    uint8_t* currentRegisterValue = 0;
	    // contains the value of ODR to be written to register
	    uint8_t ODRRegisterValue = 0;

	    ODRRegisterValue = ODRValue;

	    // read the current register values for the CTRL_X register
		HAL_I2C_Master_Transmit(&i2c_,GYR_ADDRESS, commands,1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&i2c_,GYR_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

		// set the FSR bits to the desired value without touching the other bits in the register
		commands[1] = (currentRegisterValue[0] & 0x0f) | ODRRegisterValue ;

		// write to register with the desired bit values
		HAL_I2C_Master_Transmit(&i2c_,GYR_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);
}

void LSM6DSOX::SetFSR(SensorConfiguration::FSR FSRValue){
	static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};
    commands[0] = LSM6DSOX_CTRL2_G;

    //  used to store the original register values
    uint8_t* currentRegisterValue = 0;
    // contains the value of ODR to be written to register
    uint8_t FSRRegisterValue = 0;

    FSRRegisterValue = FSRValue;

    // read the current register values for the CTRL_X register
	HAL_I2C_Master_Transmit(&i2c_,GYR_ADDRESS, commands,1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&i2c_,GYR_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

	// set the FSR bits to the desired value without touching the other bits in the register
	commands[1] = (currentRegisterValue[0] & 0xF3) | FSRRegisterValue ;

	// write to register with the desired bit values
	HAL_I2C_Master_Transmit(&i2c_,GYR_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

}


bool LSM6DSOX::WriteReg(uint8_t addr_i2c, uint8_t addr_reg, uint8_t register_data){
    uint8_t data[2] = {addr_reg, register_data};
    return HAL_I2C_Master_Transmit(&hi2c1,addr_i2c, data,2, HAL_MAX_DELAY);
}



bool LSM6DSOX::Receive(uint8_t data_buf[]){
	uint8_t addr_reg = LSM6DSOX_OUTX_L_G;
	if(HAL_I2C_Master_Transmit(&hi2c1, GYR_ADDRESS, &addr_reg, 1, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}
	if(HAL_I2C_Master_Receive(&hi2c1, GYR_ADDRESS, data_buf, 6, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}

	return true;
}

}
