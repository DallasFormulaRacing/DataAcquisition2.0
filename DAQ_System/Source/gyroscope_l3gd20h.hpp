#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include "mbed.h"
#include "L3GD20H.h"
#include "igyroscope.hpp"


namespace adapter {

class Gyroscope_L3GD20H: public IGyroscope{
    public:

        Gyroscope_L3GD20H(PinName sda, PinName scl);
        virtual ~Gyroscope_L3GD20H();

        virtual void init(PinName sda, PinName scl)override;
        virtual void GyroscopeOffset(short L3GD20HDataArrayOffset[3])override;
        virtual bool ComputeDegreesPerSecond(short L3GD20HDataArray[3], short GyroScopeDataArrayOffset[3])override;
        virtual bool ComputeRadiansPerSecond(float L3GD20HDataArray[3], short GyroScopeDataArrayOffset[3])override;

    private:
        static constexpr float DPS250Sensitivity = .00875f;
        static constexpr float DegreesToRadians = 0.01745329251f;
        L3GD20H _gyroscope;
    };
}

#endif
