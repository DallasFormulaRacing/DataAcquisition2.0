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

#include "mbed.h"
#include "igyroscope.hpp"

#define L3GD20_CTRL_REG1     0x20
#define L3GD20_OUT_X_L       0x28
#define GYR_ADDRESS          0xD6

namespace adapter {

class Gyroscope_L3GD20H: public IGyroscope{
    public:

        Gyroscope_L3GD20H(PinName sda, PinName scl);
        virtual ~Gyroscope_L3GD20H() = default;

        virtual short* GetDegreesPerSecond() override;
        virtual float* GetRadiansPerSecond() override;
        virtual bool ComputeAngularVelocity() override;

    private:
        bool ReadRawData(short g[3]);

        bool WriteReg(int addr_i2c,int addr_reg, char v);
        bool ReadReg(int addr_i2c,int addr_reg, char *v);
        bool Receive(char sad, char sub, char *buf, int length);
        void ComputeInitialOffset();

        static constexpr float DPS_250_Sensitivity = .00875f;
        static constexpr float DegreesToRadians = 0.01745329251f; //Conversion facotr of (2pi/360)

        I2C i2c_bus_;
        // First element is the x-axis
        // Second element is the y-axis
        // Third element is the z-axis
        short degrees_per_second_[3] = {0};
        float radians_per_second_[3] = {0};
        short angular_velocity_[3] = {0};
        short offset_average_[3] = {0};   
};

}

#endif // GYROSCOPE_L3GD20H_H
            