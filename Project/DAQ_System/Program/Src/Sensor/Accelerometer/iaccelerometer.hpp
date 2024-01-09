/*
* Accelerometer
* Author:        Cristian Cruz
* Email:         cris14.alex@gmail.com
*
* (c) 2023 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IACCELEROMETER
#define IACCELEROMETER

namespace sensor{

class IAccelerometer {
    public:
        virtual ~IAccelerometer() = default;

        virtual void init() = 0;
        virtual void calibrate() = 0;

        virtual void ComputeAcceleration() = 0;
        virtual void ComputeMagnetometer() = 0;


        virtual float* GetAcceleration() = 0;
        virtual float* GetMagnetometerData() = 0;
};

}
#endif // IACCELEROMETER
