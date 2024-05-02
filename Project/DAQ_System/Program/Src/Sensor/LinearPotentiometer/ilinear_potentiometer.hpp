/*
* Linear Potentiometer Abstract Interface
* Author:   Manuel DJC
*           Cristian Cruz
*
* Email:    Manuel.DeJesusContreras@UTDallas.edu
*           cris14.alex@gmail.com
*
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef ILINEAR_POTENTIOMETER_H
#define ILINEAR_POTENTIOMETER_H

namespace sensor {

class ILinearPotentiometer {
    public:
        virtual ~ILinearPotentiometer() = default;

        virtual void DisplacementInches(float quantized_counts[]) = 0;
        virtual void DisplacementMillimeters(float quantized_counts[]) = 0;
};

}

#endif // ILINEAR_POTENTIOMETER_H
