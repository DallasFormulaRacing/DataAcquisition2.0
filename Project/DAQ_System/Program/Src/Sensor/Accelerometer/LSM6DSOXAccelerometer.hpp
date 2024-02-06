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

#ifndef ACCELEROMETER_LSM6DSOX_H
#define ACCELEROMETER_LSM6DSOX_H


// DFR Custom Dependancies
#include "iaccelerometer.hpp"

// ST HAL Dependencies
#include "i2c.h"

#define ACC_ADDRESS  0xd4

// Register Addresses

#define CTRL1_XL          0x10  // CTRL_REG1_A

#define OUT_X_L_A         0x28

namespace sensor{

class LSM6DSOX: public IAccelerometer {
    public:
		/// @param hi2c an I2C peripheral from ST's HAL
		LSM6DSOX(I2C_HandleTypeDef &hi2c);
        virtual ~LSM6DSOX() = default;

        // Initialize the device
        void init() override;

        // returns the value of the accelerometer at the given moment
        // @return -19.6 to 19.6 (  ±2g  )
        float* GetAcceleration() override;


        // calculates the acceleration in terms of m/s^2
        void ComputeAcceleration() override;

    private:

        // averages the idle value of the accelerometer to calibrate
        void calibrate() override;


        // reads raw values from the accelerometer
        // @return -32768 to 32767
        void ReadRawAcceleration();


        // The I2C peripherals from ST's HAL library
        I2C_HandleTypeDef i2c_;


        // value to hold offset values, scale, and gravity
        int offset_[3];
        int scale_[3];
        float gravity_adjustment_conversion_factor_;


        // data to hold accelerometer values in 16 bit form or in m/s^2 form
        short raw_acceleration_data_[3] = {0, 0, 0};
        float real_acceleration_data_[3] = {0, 0, 0};

};

}
#endif // ACCELEROMETER_LSM6DSOX_H
