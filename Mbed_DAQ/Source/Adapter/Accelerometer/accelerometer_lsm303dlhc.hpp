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

#ifndef ACCELEROMETER_LSM303DLHC_H
#define ACCELEROMETER_LSM303DLHC_H

// External Dependancies
#include "mbed.h"

// DFR Custom Dependancies
#include "Adapter/Interfaces/iaccelerometer.hpp"


namespace adapter {

#define MAG_ADDRESS  0x3C
#define ACC_ADDRESS  0x32

// Register Addresses
#define MR_REG_M          0x02

#define CRA_REG_M         0x00
#define CTRL_REG1_A       0x20

#define OUT_X_L_A         0x28
#define OUT_X_H_M         0x03

class Accelerometer_LSM303DLHC: public IAccelerometer {
    public:
        Accelerometer_LSM303DLHC(PinName sda, PinName scl);
        // Initialize the device
        void init() override;

        float* GetAcceleration() override;
        float* GetMagnetometerData() override;
        
        void ComputeAcceleration() override;
        void ComputeMagnetometer() override;


//========================================Todo & Unused=======================================
        /** sets the x, y, and z offset corrections for hard iron calibration
         * 
         * Calibration details here:
         *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
         *
         * If you gather raw magnetometer data and find, for example, x is offset
         * by hard iron by -20 then pass +20 to this member function to correct
         * for hard iron.
         *
         * @param x is the offset correction for the x axis
         * @param y is the offset correction for the y axis
         * @param z is the offset correction for the z axis
         */
        
        void SetOffset(float x, float y, float z);
        
        /** sets the scale factor for the x, y, and z axes
         *
         * Calibratio details here:
         *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
         *
         * Sensitivity of the three axes is never perfectly identical and this
         * function can help to correct differences in sensitivity.  You're
         * supplying a multipler such that x, y and z will be normalized to the
         * same max/min values
         */
        void SetScale(float x, float y, float z);
//==============================================================================================

    private:
        void calibrate() override;
        void ReadRawAcceleration();
        void ReadRawMagnetometer();

        I2C i2c_bus_;
        int offset_[3];
        int scale_[3];
        float gravity_adjustment_conversion_factor_;

        short raw_acceleration_data_[3] = {0, 0, 0};
        float real_acceleration_data_[3] = {0, 0, 0};

        short raw_magnetometer_data_[3] = {0, 0, 0};
        float real_magnetometer_data_[3] = {0, 0, 0};
};

}

#endif // ACCELEROMETER_LSM303DLHC_H
