#include "mbed.h"
#include "accelerometer_lsm303dlhc.h"
 
Accelerometer_LSM303DLHC::Accelerometer_LSM303DLHC(PinName sda, PinName scl)
  : i2c_bus_(sda, scl) {
    // the I2C bus/clock frequency, either standard (100000) or fast (400000)
    i2c_bus_.frequency(400000);
}
 
void Accelerometer_LSM303DLHC::init() {
    static constexpr uint8_t kNumBytes = 2;
    char commands[kNumBytes] = "";

    // init mag
    // continuous conversion mode
    commands[0] = MR_REG_M;
    commands[1] = 0x00;
    i2c_bus_.write(MAG_ADDRESS, commands, kNumBytes);
    
    // data rate 75hz
    commands[0] = CRA_REG_M;
    commands[1] = 0x18; // 0b00011000
    i2c_bus_.write(MAG_ADDRESS, commands, kNumBytes);
    
    // init acc
    // data rate 100hz 
    commands[0] = CTRL_REG1_A;
    commands[1] = 0x2F; // 0b00101111
    i2c_bus_.write(ACC_ADDRESS, commands, kNumBytes);

    calibrate();
}

void Accelerometer_LSM303DLHC::calibrate() {
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
    // determined convesion factor
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

void Accelerometer_LSM303DLHC::SetScale(float x, float y, float z) {
    scale_[0] = x;
    scale_[1] = y;
    scale_[2] = z;
}
 
void Accelerometer_LSM303DLHC::SetOffset(float x, float y, float z) {
    offset_[0] = x;
    offset_[1] = y;
    offset_[2] = z;
}

float* Accelerometer_LSM303DLHC::GetAcceleration() {
    return real_acceleration_data_;
}

void Accelerometer_LSM303DLHC::ComputeAcceleration() {
    ReadRawAcceleration();
    
    static constexpr float kMG_LSB = 0.001F;
    real_acceleration_data_[0] = raw_acceleration_data_[0] * kMG_LSB / gravity_adjustment_conversion_factor_;
    real_acceleration_data_[1] = raw_acceleration_data_[1] * kMG_LSB / gravity_adjustment_conversion_factor_;
    real_acceleration_data_[2] = raw_acceleration_data_[2] * kMG_LSB / gravity_adjustment_conversion_factor_;
}

void Accelerometer_LSM303DLHC::ReadRawAcceleration() {
    char command[1] = { OUT_X_L_A | (1<<7) };
    i2c_bus_.write(ACC_ADDRESS, command, 1);

    char bytes_received[6];
    i2c_bus_.read(ACC_ADDRESS, bytes_received, 6);
 
    // 12-bit values
    raw_acceleration_data_[0] = (short)(bytes_received[1]<<8 | bytes_received[0]) >> 4;
    raw_acceleration_data_[1] = (short)(bytes_received[3]<<8 | bytes_received[2]) >> 4;
    raw_acceleration_data_[2] = (short)(bytes_received[5]<<8 | bytes_received[4]) >> 4;
}

//======================================================================================

float* Accelerometer_LSM303DLHC::GetMagnetometerData() {
    // TODO: Rename this method after the unit returned by the magnetometer
    return real_acceleration_data_;
}
 
void Accelerometer_LSM303DLHC::ReadRawMagnetometer() {
    char command[1] = {OUT_X_H_M};
    i2c_bus_.write(MAG_ADDRESS, command, 1);

    char bytes_received[6];
    i2c_bus_.read(MAG_ADDRESS, bytes_received, 6);
    
    raw_magnetometer_data_[0] = (short) (bytes_received[0]<<8 | bytes_received[1]); // X
    raw_magnetometer_data_[1] = (short) (bytes_received[4]<<8 | bytes_received[5]); // Y
    raw_magnetometer_data_[2] = (short) (bytes_received[2]<<8 | bytes_received[3]); // Z
 }

 void Accelerometer_LSM303DLHC::ComputeMagnetometer() {
     // TODO: post-process raw_magnetometer_data_ & store results in real_magnetometer_data_
 }
