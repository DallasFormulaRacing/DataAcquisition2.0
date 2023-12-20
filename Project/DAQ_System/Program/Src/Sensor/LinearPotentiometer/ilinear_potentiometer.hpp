/*
* Linear Potentiometer SLS1300
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

        virtual float DisplacementInches() = 0;
        virtual float DisplacementMillimeters() = 0;
};

}

#endif // ILINEAR_POTENTIOMETER_H
