
#ifndef IGYROSCOPE
#define IGYROSCOPE

#include "mbed.h"
#include "L3GD20H.h"

namespace adapter{

class IGyroscope{
    public:
        virtual void init(PinName sda, PinName scl) = 0;
        virtual ~IGyroscope() = default;

        virtual void GyroscopeOffset(short GyroScopeDataArrayOffset[3]) = 0;
        virtual bool ComputeDegreesPerSecond(short GyroscopeDataArray[3],short GyroScopeDataArrayOffset[3]) = 0;
        virtual bool ComputeRadiansPerSecond(float GyroscopeDataArray[3], short GyroScopeDataArrayOffset[3]) = 0;
};
}

#endif 