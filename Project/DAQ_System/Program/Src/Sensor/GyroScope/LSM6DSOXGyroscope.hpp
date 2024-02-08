/*
* Gyroscope LSM6DSOX
* Authors:       William Lim
*
* Email:         wll180000@utdallas.edu
*
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef GYROSCOPE_LSM6DSOX_H
#define GYROSCOPE_LSM6DSOX_H

// ST HAL Dependencies
#include "i2c.h"

// DFR Custom Dependencies
#include "igyroscope.hpp"

#include <stdint.h>
#define LSM6DSOX_CTRL2_G        0x11
#define LSM6DSOX_OUTX_L_G       0x22
#define GYR_ADDRESS             0xD4

enum ODR{
	ODR12_5,
	ODR26,
	ODR52,
	ODR104,
	ODR208,
	ODR416,
	ODR833,
	ODR1_66K,
	ODR3_33K,
	ODR6_66K,
};

enum FSR{
	DPS250 = 10,
	DPS500,
	DPS1000,
	DPS2000,
};

namespace sensor{

class LSM6DSOX: public IGyroscope{
    public:
		/// @param hi2c an I2C peripheral from ST's HAL.
        LSM6DSOX(I2C_HandleTypeDef &hi2c);
        virtual ~LSM6DSOX() = default;

        /// Converts the raw values from the gyroscope to Degrees/second
        /// @return ± 250 on each axis
        virtual short* DegreesPerSecond() override;
        /// Converts the raw values from the gyroscope to Radians/second
        /// @return ± 4.363 on each axis
        virtual float* RadiansPerSecond() override;

        /// sets the ODR of the gyroscope
        /// @param value from ODR enumeration
        void SetODR(ODR ODRValue);

        /// sets the FSR of the gyroscope
        /// @param value from FSR enumeration
        void SetFSR(FSR FSRValue);

    private:
        /// takes in raw data from the gyroscope
        /// @param g[3] array to hold data for each axis
        /// @return -32768 to 32767 on each axis
        bool ReadRawData(short g[3]);

        /// takes in raw data from the gyroscope
        /// @return -32768 to 32767 on each axis
        virtual bool AngularVelocity() override;

        /// writes to a specified value to a specified register
        /// @param addr_i2c 8-bit slave address( 7-bit slave address << 1 )
        /// @param addr_reg 8-bit address of register to write to
        /// @param register_data 8-bit value to write to selected register
        /// @return write status (true or false)
        bool WriteReg(uint8_t addr_i2c, uint8_t addr_reg, uint8_t register_data);


        /// reads values from the selected register
        /// @param  addr_i2c 8-bit slave address( 7-bit slave address << 1 )
        /// @param  addr_reg 8-bit address of register to write to
        /// @param  read_data is array for storing the read content of the selected register
        /// @return read status (true or false)
        bool ReadReg(uint8_t addr_i2c,uint8_t *addr_reg, uint8_t read_data[]);


        /// sequential reading function for LSM6DSOX. Reads all the registers that contain axis
        /// data. [0x22, 0x23, 0x24, 0x25, 0x26, 0x27]
        /// @param  data_buf array to contain all the raw axis data from l3gd20h
        /// @return status of function(true or false)
        bool Receive(uint8_t data_buf[]);

        /// averages the stationary values for each axis
        /// @return None
        void ComputeInitialOffset();

        // initialization register for CTRL1 register initializes x,y, and z axis and PD to 1
        uint8_t CTRL2_G_init_value = 0x50;

        // FSR value given by the data sheet. The values is used to normalize the raw data
        // to ± 250 degrees
        static constexpr float DPS_250_Sensitivity = .00875f;

        //Conversion factor of (2pi/360) to convert to radians
        static constexpr float DegreesToRadians = 0.01745329251f; //Conversion facotr of (2pi/360)

        // First element is the x-axis
        // Second element is the y-axis
        // Third element is the z-axis
        short degrees_per_second_[3] = {0};
        float radians_per_second_[3] = {0};
        short angular_velocity_[3] = {0};
        short offset_average_[3] = {0};

        /// The I2C peripheral from ST's HAL library.
        I2C_HandleTypeDef& hi2c_;
};

}

#endif // GYROSCOPE_LSM6DSOX_H
