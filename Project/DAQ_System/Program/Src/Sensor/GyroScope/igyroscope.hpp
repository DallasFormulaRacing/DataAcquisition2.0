/*
* Gyroscope
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


#ifndef IGYROSCOPE_H
#define IGYROSCOPE_H



class IGyroscope{
    public:
        virtual ~IGyroscope() = default;

        virtual short* GetDegreesPerSecond() = 0;
        virtual float* GetRadiansPerSecond() = 0;
        virtual bool ComputeAngularVelocity() = 0;
};


#endif // IGYROSCOPE_H
