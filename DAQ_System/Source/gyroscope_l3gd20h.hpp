
#ifndef __L3GD20H_H
#define __L3GD20H_H

#include "mbed.h"
#include "igyroscope.hpp"

#define L3GD20_CTRL_REG1     0x20
#define L3GD20_OUT_X_L       0x28
#define GYR_ADDRESS          0xD6

namespace adapter{
class Gyroscope_L3GD20H: public IGyroscope{
    public:

        Gyroscope_L3GD20H(PinName sda, PinName scl);
        virtual ~Gyroscope_L3GD20H() = default;

        bool read(short g[3]);
        virtual bool ComputeDegreesPerSecond(short L3GD20HDataArray[3]) override;
        virtual bool ComputeRadiansPerSecond(float L3GD20HDataArray[3]) override;

        
    private:
            I2C _L3GD20H_;
        short gx, gy, gz;
        static constexpr float DPS250Sensitivity = .00875f;
        static constexpr float DegreesToRadians = 0.01745329251f;
        short offsetAverage[3];

        bool write_reg(int addr_i2c,int addr_reg, char v);
        bool read_reg(int addr_i2c,int addr_reg, char *v);
        bool recv(char sad, char sub, char *buf, int length);
        void gyro_offset(short offsetAverage[3]);
};
}
#endif
            