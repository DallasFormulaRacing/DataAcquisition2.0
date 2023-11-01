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

#ifndef ILINEAR_POTENTIOMETER_H
#define ILINEAR_POTENTIOMETER_H

namespace adapter {

class ILinearPotentiometer {
    public:
        virtual ~ILinearPotentiometer() = default;
        
        virtual float GetDisplacementInches() = 0;
        virtual float GetDisplacementMillimeters() = 0;
        
        virtual void ComputeDisplacementPercentage() = 0;
};

}

#endif // ILINEAR_POTENTIOMETER_H
