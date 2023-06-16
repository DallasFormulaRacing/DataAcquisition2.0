#include "mbed.h"
#include "LSM303DLHC.h"
#include "stdio.h"
#include <cstdint>
#include <locale>
 
#define MAG_ADDRESS  0x3C
#define ACC_ADDRESS  0x32
  
 
LSM303DLHC::LSM303DLHC(PinName sda, PinName scl)
  : device_(sda, scl) {
    // the I2C bus/clock frequency, either standard (100000) or fast (400000)
    device_.frequency(400000);
}
 
void LSM303DLHC::init() {
    // init mag
    // continuous conversion mode
    data_[0] = MR_REG_M;
    data_[1] = 0x00;
    device_.write(MAG_ADDRESS, data_, 2);
    // data rate 75hz
    data_[0] = CRA_REG_M;
    data_[1] = 0x18; // 0b00011000
    device_.write(MAG_ADDRESS, data_, 2);
    // init acc
    // data rate 100hz 
    data_[0] = CTRL_REG1_A;
    data_[1] = 0x2F; // 0b00101111
    device_.write(ACC_ADDRESS, data_, 2);
}
 
void LSM303DLHC::Read(int a[3], int m[3]) {
    ReadRawAcceleration(a);
    ReadRawMagnetometer(m);   
}
 
void LSM303DLHC::ReadRawAcceleration(int a[3]) {
    data_[0] = OUT_X_L_A | (1<<7);
    device_.write(ACC_ADDRESS, data_, 1);
    device_.read(ACC_ADDRESS, data_, 6);
 
    // 12-bit values
    a[0] = (short)(data_[1]<<8 | data_[0]) >> 4;
    a[1] = (short)(data_[3]<<8 | data_[2]) >> 4;
    a[2] = (short)(data_[5]<<8 | data_[4]) >> 4;
}
 
void LSM303DLHC::ReadRawMagnetometer(int m[3]) {
    data_[0] = OUT_X_H_M;
    device_.write(MAG_ADDRESS, data_, 1);
    device_.read(MAG_ADDRESS, data_, 6);
    
    m[0] = (short) (data_[0]<<8 | data_[1]); // X
    m[1] = (short) (data_[4]<<8 | data_[5]); // Y
    m[2] = (short) (data_[2]<<8 | data_[3]); // Z
 }

void LSM303DLHC::SetScale(float x, float y, float z) {
    scale_[0] = x;
    scale_[1] = y;
    scale_[2] = z;
}
 
void LSM303DLHC::setOffset(float x, float y, float z) {
    offset_[0] = x;
    offset_[1] = y;
    offset_[2] = z;
}


static constexpr float _lsm303Accel_MG_LSB = 0.001F;

void LSM303DLHC::computeAcc(int raw_data[3], float real_data[3], float gravity_adjustment_conversion_factor) {
    real_data[0] = raw_data[0] * _lsm303Accel_MG_LSB / gravity_adjustment_conversion_factor;
    real_data[1] = raw_data[1] * _lsm303Accel_MG_LSB / gravity_adjustment_conversion_factor;
    real_data[2] = raw_data[2] * _lsm303Accel_MG_LSB / gravity_adjustment_conversion_factor;
}

double LSM303DLHC::calibrate() {
    int acc_array[3] = {0,0,0};
    constexpr uint8_t sampleCount = 100;

    double acc_magnitude = 0;
    double average_array[sampleCount];
    

    //collects a number of samples to calibrate for 1G
    for(int i = 0; i < sampleCount; i++) {
        ReadRawAcceleration(acc_array);
        acc_magnitude = sqrt(acc_array[0] * acc_array[0] + acc_array[1] * acc_array[1] + acc_array[2] * acc_array[2]);
        average_array[i] = acc_magnitude * 0.001;
    }


    double min_value = std::numeric_limits<double>::max();
    double max_value = -1;
    
    //finds the largest and smallest magnitudes
    for(int j = 2; j < sampleCount; j++) {
        if(average_array[j] <= min_value) {
            min_value = average_array[j];
        
        } else if (average_array[j] > max_value) {
            max_value = average_array[j];
        }
    }

    /*checks that the maximum deviation is below a certain threshold. If it is too large, the conversion factor
    * will be set to a previously determined convesion factor */
    double gravity_adjustment_conversion_factor = 0;

    if (max_value - min_value < 0.09) {
        double total_array = 0;
	    for(int i = 0; i < sampleCount; i++) {
		    total_array += average_array[i];
	    }

	    gravity_adjustment_conversion_factor = total_array / sampleCount;
    } else {
        gravity_adjustment_conversion_factor = 1.028;
    }

    return gravity_adjustment_conversion_factor;
}