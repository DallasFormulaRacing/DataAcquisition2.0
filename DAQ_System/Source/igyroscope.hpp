
#ifndef IGYROSCOPE
#define IGYROSCOPE

#include "mbed.h"

namespace adapter{

class IGyroscope{
    public:
        virtual ~IGyroscope() = default;

        virtual bool ComputeDegreesPerSecond(short GyroscopeDataArray[3]) = 0;
        virtual bool ComputeRadiansPerSecond(float GyroscopeDataArray[3]) = 0;
};
}

#endif 