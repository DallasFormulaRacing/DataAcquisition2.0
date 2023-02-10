/*
* Linear Potentiometer
* Authors:        Cristian Cruz
*                 Manuel DJC
*
* Email:          cris14.alex@gmail.com
*                 Manuel.DeJesusContreras@UTDallas.edu
* 
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef ILINEAR_POTENTIOMETER
#define ILINEAR_POTENTIOMETER

namespace adapter {

class ILinear_Potentiometer {
    public:
        virtual ~ILinear_Potentiometer() = default;
        
        virtual double GetDisplacementPercentage() = 0;
        virtual void ComputeDisplacementPercentage() = 0;
};

}

#endif // ILINEAR_POTENTIOMETER
