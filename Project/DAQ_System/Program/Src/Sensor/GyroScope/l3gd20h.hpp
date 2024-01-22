/*
* Gyroscope L3GD20H
* Authors:       William Lim
*                Alex Shi
* Modified By:   Cristian Cruz
*
* Email:         wll180000@utdallas.edu
*                ahs190001@utdallas.edu
*                cris14.alex@gmail.com
*
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef GYROSCOPE_L3GD20H_H
#define GYROSCOPE_L3GD20H_H

// ST HAL Dependencies
#include "i2c.h"

// DFR Custom Dependencies
#include "igyroscope.hpp"

#include <stdint.h>
#define L3GD20_CTRL_REG1     0x20
#define L3GD20_OUT_X_L       0x28
#define GYR_ADDRESS          0xD6

namespace sensor{

class L3GD20H: public IGyroscope{
    public:
		/// @param hi2c an I2C peripheral from ST's HAL.
        L3GD20H(I2C_HandleTypeDef &hi2c);
        virtual ~L3GD20H() = default;

        /// Converts the raw values from the gyroscope to Degrees/second
        /// @return ± 250 on each axis
        virtual short* DegreesPerSecond() override;
        /// Converts the raw values from the gyroscope to Radians/second
        /// @return ± 4.363 on each axis
        virtual float* RadiansPerSecond() override;

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


        /// sequential reading function for l3g20h. Reads all the registers that contain axis
        /// data. [0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D]
        /// @param  data_buf array to contain all the raw axis data from l3gd20h
        /// @return status of function(true or false)
        bool Receive(uint8_t data_buf[]);

        /// averages the stationary values for each axis
        /// @return None
        void ComputeInitialOffset();

        // initialization register for CTRL1 register initializes x,y, and z axis and PD to 1
        uint8_t ctrl_reg1_init_value = 0x0F;

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

#endif // GYROSCOPE_L3GD20H_H
