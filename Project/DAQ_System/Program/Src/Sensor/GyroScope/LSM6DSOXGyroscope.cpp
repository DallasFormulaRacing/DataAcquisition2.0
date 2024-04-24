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

LSM6DSOX_Gyroscope::LSM6DSOX_Gyroscope(I2C_HandleTypeDef& hi2c):i2c_(hi2c){
    WriteReg(LSM6DSOX_GYR_ADDRESS, LSM6DSOX_CTRL2_G, CTRL2_G_init_value);
    ComputeInitialOffset();
}

bool LSM6DSOX_Gyroscope::ReadRawData(short arr[3]) {
    uint8_t bytes[6] = {0};
    uint8_t addr =  LSM6DSOX_OUTX_L_G; // addr needs to be a reference so assigning define to a variable to reference

    HAL_I2C_Master_Transmit(&i2c_, LSM6DSOX_GYR_ADDRESS, &addr, 1, HAL_MAX_DELAY);

    // sequential reading function for LSM6DSOX. Reads all the registers that contain axis
    // data. [0x22, 0x23, 0x24, 0x25, 0x26, 0x27]
    if(HAL_I2C_Master_Receive(&i2c_, LSM6DSOX_GYR_ADDRESS, bytes, 6, HAL_MAX_DELAY) != HAL_OK){

        arr[0] = (bytes[1] << 8 | bytes[0]);
        arr[1] = (bytes[3] << 8 | bytes[2]);
        arr[2] = (bytes[5] << 8 | bytes[4]);

        return true;
	}

    return false;
}

bool LSM6DSOX_Gyroscope::AngularVelocity() {
    short raw_data[3] = {0};
    if (ReadRawData(raw_data)) {
        angular_velocity_[0] =  raw_data[0] - offset_average_[0];
        angular_velocity_[1] =  raw_data[1] - offset_average_[1];
        angular_velocity_[2] =  raw_data[2] - offset_average_[2];

        return true;
    }

    return false;
}

short* LSM6DSOX_Gyroscope::DegreesPerSecond() {
    // Equation: degrees/second = RawData * SensitivityOfFullScaleRange
    // Using a default sensitivity for L3GD20H is 250dps
	AngularVelocity();
    degrees_per_second_[0] = angular_velocity_[0] * SensitivityFactor;
    degrees_per_second_[1] = angular_velocity_[1] * SensitivityFactor;
    degrees_per_second_[2] = angular_velocity_[2] * SensitivityFactor;

    return degrees_per_second_;
}

float* LSM6DSOX_Gyroscope::RadiansPerSecond() {
	AngularVelocity();
    radians_per_second_[0] = angular_velocity_[0] * SensitivityFactor * DegreesToRadians;
    radians_per_second_[1] = angular_velocity_[1] * SensitivityFactor * DegreesToRadians;
    radians_per_second_[2] = angular_velocity_[2] * SensitivityFactor * DegreesToRadians;

    return radians_per_second_;
}

void LSM6DSOX_Gyroscope::ComputeInitialOffset(){
    short temp_data[3] = {0};
    short sum[3] = {0};
    uint8_t average_sample_size = 255;

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

void LSM6DSOX_Gyroscope::SetODR(ODR SelectedODRValue){

		static constexpr uint8_t kNumBytes = 2;
	    uint8_t commands[kNumBytes] = {0};
	    commands[0] = LSM6DSOX_CTRL2_G;

	    // used to store the original register values
	    uint8_t* currentRegisterValue = 0;
	    // contains the value of ODR to be written to register

	    // read the current register values for the CTRL_X register
		HAL_I2C_Master_Transmit(&i2c_,LSM6DSOX_GYR_ADDRESS, commands,1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&i2c_,LSM6DSOX_GYR_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

		// set the FSR bits to the desired value without touching the other bits in the register
		commands[1] = (currentRegisterValue[0] & 0x0f) | static_cast<uint8_t>(SelectedODRValue) << 4 ;

		// write to register with the desired bit values
		HAL_I2C_Master_Transmit(&i2c_,LSM6DSOX_GYR_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);
}

void LSM6DSOX_Gyroscope::SetFSR(FSR SelectedFSRValue){

	static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};
    commands[0] = LSM6DSOX_CTRL2_G;

    //  used to store the original register values
    uint8_t currentRegisterValue[1] = {0};

    // read the current register values for the CTRL_X register

	HAL_I2C_Master_Transmit(&i2c_,LSM6DSOX_GYR_ADDRESS, commands,1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&i2c_,LSM6DSOX_GYR_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

	// set the FSR bits to the desired value without touching the other bits in the register
	commands[1] = (currentRegisterValue[0] & 0xF3) | (static_cast<uint8_t>(SelectedFSRValue) << 2 );

	// write to register with the desired bit values
	HAL_I2C_Master_Transmit(&i2c_,LSM6DSOX_GYR_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

	// setting up sensitivity factor based on the selected FSR Value. used to convert raw values to degrees
	switch(SelectedFSRValue){
	case FSR::DPS250:
		SensitivityFactor = 0.00875;
		break;
	case FSR::DPS500:
		SensitivityFactor = 0.01750;
		break;
	case FSR::DPS1000:
		SensitivityFactor =  0.0350;
		break;
	case FSR::DPS2000:
		SensitivityFactor = 0.0700;
		break;

	}

}


bool LSM6DSOX_Gyroscope::WriteReg(uint8_t addr_i2c, uint8_t addr_reg, uint8_t register_data){
    uint8_t data[2] = {addr_reg, register_data};
    return HAL_I2C_Master_Transmit(&i2c_,addr_i2c, data,2, HAL_MAX_DELAY);
}



}
