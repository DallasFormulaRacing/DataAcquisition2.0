/*
* Accelerometer LSM303DLHC
* Authors:       Nathaniel Ho
*                Cristian Cruz
*                Chad Melhem
*
* Email:         nathanielho712@gmail.com
*                cris14.alex@gmail.com
*                chad.melhem@utdallas.edu
*
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include "lsm303dlhc.hpp"
#include <stdint.h>
#include <cmath>
#include <limits>


namespace sensor{

LSM303DLHC::LSM303DLHC(I2C_HandleTypeDef& hi2c):i2c_(hi2c){}

void LSM303DLHC::init() {
    static constexpr uint8_t kNumBytes = 2;
    uint8_t commands[kNumBytes] = {0};

    // init mag
    // continuous conversion mode
    commands[0] = MR_REG_M;
    commands[1] = 0x00;
    HAL_I2C_Master_Transmit(&i2c_,MAG_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

    // data rate 75hz
    commands[0] = CRA_REG_M;
    commands[1] = 0x18; // 0b00011000
    HAL_I2C_Master_Transmit(&i2c_,MAG_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

    // init acc
    // data rate 100hz
    commands[0] = CTRL_REG1_A;
    commands[1] = 0x57; // 0b01010111
    HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

    // High Resolution mode (HR) enable
    commands[0] = CTRL_REG4_A;
    commands[1] = 0x08; // 0b00001000
    HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, commands,kNumBytes, HAL_MAX_DELAY);

    calibrate();
}

void LSM303DLHC::calibrate() {
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

void LSM303DLHC::SetScale(float x, float y, float z) {
    scale_[0] = x;
    scale_[1] = y;
    scale_[2] = z;
}

void LSM303DLHC::SetOffset(float x, float y, float z) {
    offset_[0] = x;
    offset_[1] = y;
    offset_[2] = z;
}

float* LSM303DLHC::GetAcceleration() {
    return real_acceleration_data_;
}

void LSM303DLHC::ComputeAcceleration() {
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

void LSM303DLHC::ReadRawAcceleration() {
	static constexpr int ByteArraySize = 6;
	static constexpr int I2CWriteSize = 1;
	static constexpr int I2CReadSize = 6;

    uint8_t command[1] = { OUT_X_L_A | 0x80 };
    HAL_I2C_Master_Transmit(&i2c_,ACC_ADDRESS, command,I2CWriteSize, HAL_MAX_DELAY);

    uint8_t bytes_received[ByteArraySize];
    HAL_I2C_Master_Receive(&i2c_,ACC_ADDRESS, bytes_received,I2CReadSize, HAL_MAX_DELAY);

    // 16-bit values
    raw_acceleration_data_[0] = (bytes_received[1] <<8 | bytes_received[0]);
    raw_acceleration_data_[1] = (bytes_received[3] <<8 | bytes_received[2]);
    raw_acceleration_data_[2] = (bytes_received[5] <<8 | bytes_received[4]);
}

}





