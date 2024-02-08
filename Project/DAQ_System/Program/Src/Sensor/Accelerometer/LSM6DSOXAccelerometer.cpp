/*
* Accelerometer LSM6DSOX
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
#include <cmath>
#include <limits>

// DFR Custom dependencies
#include "LSM6DSOXAccelerometer.hpp"



namespace sensor{

LSM6DSOX::LSM6DSOX(I2C_HandleTypeDef& hi2c):i2c_(hi2c){}

void LSM6DSOX::init() {
    static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};

    // data rate 104hz
    commands[0] = CTRL1_XL;
    commands[1] = 0x40; // 0b01000000
    HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

    calibrate();
}

void LSM6DSOX::calibrate() {
    static constexpr uint8_t kSampleCount = 100;
    double acc_magnitude = 0;
    double average_array[kSampleCount];

    // Collects samples to calibrate for 1G
    for(int i = 0; i < kSampleCount; i++) {
        ReadRawAcceleration();
        acc_magnitude = sqrt(raw_acceleration_data_[0] * raw_acceleration_data_[0] + raw_acceleration_data_[1] * raw_acceleration_data_[1] + raw_acceleration_data_[2] * raw_acceleration_data_[2]);
        average_array[i] = acc_magnitude * 0.001;
    }

    double min_value = std::numeric_limits<double>::max();
    double max_value = -1;

    // Find the largest and smallest magnitudes
    for(int j = 2; j < kSampleCount; j++) {
        if(average_array[j] <= min_value) {
            min_value = average_array[j];

        } else if (average_array[j] > max_value) {
            max_value = average_array[j];
        }
    }

    // Check that the maximum deviation is below a certain threshold.
    // If it is too large, the conversion factor will be set to a previously
    // determined conversion factor
    gravity_adjustment_conversion_factor_ = 0;

    if (max_value - min_value < 0.09) {
        double total_array = 0;
	    for(int i = 0; i < kSampleCount; i++) {
		    total_array += average_array[i];
	    }

	    gravity_adjustment_conversion_factor_ = total_array / kSampleCount;
    } else {
        // Testing found that ~1.028 is about 1G
        gravity_adjustment_conversion_factor_ = 1.028;
    }

}

void LSM6DSOX::SetODR(ODR ODRValue){
	static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};
    commands[0] = CTRL1_XL;
    uint8_t* currentRegisterValue = 0;
    uint8_t ODRRegisterValue = 0;

    switch(ODRValue){
		case ODR12_5:
			ODRRegisterValue = 0xB0;
			break;
		case ODR26:
			ODRRegisterValue = 0x20;
			break;
		case ODR52:
			ODRRegisterValue = 0x30;
			break;
		case ODR104:
			ODRRegisterValue = 0x40;
			break;
		case ODR208:
			ODRRegisterValue = 0x50;
			break;
		case ODR416:
			ODRRegisterValue = 0x60;
			break;
		case ODR833:
			ODRRegisterValue = 0x70;
			break;
		case ODR1_66K:
			ODRRegisterValue = 0x80;
			break;
		case ODR3_33K:
			ODRRegisterValue = 0x90;
			break;
		case ODR6_66K:
			ODRRegisterValue = 0xA0;
			break;
    }
    // read the current register values for the CTRL_X register
	HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&i2c_,ACC_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

	// set the ODR bits to the desired value without touching the other bits in the register
	commands[1] = (currentRegisterValue[0] & 0x0F) | ODRRegisterValue ;

	// write to register with the desired bit values
	HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);
}

void LSM6DSOX::SetFSR(FSR FSRValue){
	static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};
    commands[0] = CTRL1_XL;
    uint8_t* currentRegisterValue = 0;
    uint8_t FSRRegisterValue = 0;

    switch(FSRValue){
		case FSR2g:
			FSRRegisterValue = 0x00;
			break;
		case FSR4g:
			FSRRegisterValue = 0x08;
			break;
		case FSR8g:
			FSRRegisterValue = 0x0C;
			break;
		case FSR16g:
			FSRRegisterValue = 0x04;
			break;
    }

    // read the current register values for the CTRL_X register
	HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&i2c_,ACC_ADDRESS, currentRegisterValue,1, HAL_MAX_DELAY);

	// set the FSR bits to the desired value without touching the other bits in the register
	commands[1] = (currentRegisterValue[0] & 0xF3) | FSRRegisterValue ;

	// write to register with the desired bit values
	HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

}

float* LSM6DSOX::GetAcceleration() {
    return real_acceleration_data_;
}

void LSM6DSOX::ComputeAcceleration() {
    ReadRawAcceleration();
/*   old calculations for accelerometer
    static constexpr float kMG_LSB = .001F;
    real_acceleration_data_[0] = (raw_acceleration_data_[0]) * kMG_LSB / gravity_adjustment_conversion_factor_;
    real_acceleration_data_[1] = (raw_acceleration_data_[1]) * kMG_LSB / gravity_adjustment_conversion_factor_;
    real_acceleration_data_[2] = (raw_acceleration_data_[2]) * kMG_LSB / gravity_adjustment_conversion_factor_;
*/

    // linearization of the accelerometer. by default range of the accelerometer is ±2 g
    real_acceleration_data_[0] = 0.0006 *raw_acceleration_data_[0] - 0.0002;
    real_acceleration_data_[1] = 0.0006 *raw_acceleration_data_[1] - 0.0002;
    real_acceleration_data_[2] = 0.0006 *raw_acceleration_data_[2] - 0.0002;
}

void LSM6DSOX::ReadRawAcceleration() {
	static constexpr int ByteArraySize = 6;
	static constexpr int I2CWriteSize = 1;
	static constexpr int I2CReadSize = 6;

    uint8_t command[1] = { OUTX_L_A };
    HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, command,I2CWriteSize, HAL_MAX_DELAY);

    uint8_t bytes_received[ByteArraySize];
    HAL_I2C_Master_Receive(&i2c_,ACC_ADDRESS, bytes_received,I2CReadSize, HAL_MAX_DELAY);

    // 16-bit values
    raw_acceleration_data_[0] = (bytes_received[1] <<8 | bytes_received[0]);
    raw_acceleration_data_[1] = (bytes_received[3] <<8 | bytes_received[2]);
    raw_acceleration_data_[2] = (bytes_received[5] <<8 | bytes_received[4]);
}

}





