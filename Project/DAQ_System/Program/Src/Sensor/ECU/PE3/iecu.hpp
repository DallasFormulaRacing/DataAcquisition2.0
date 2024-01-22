/*
* ECU Abstract Interface
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IECU_H
#define IECU_H

namespace sensor {

class IEcu {
    public:
        virtual ~IEcu() = default;

};

}

#endif // IECU_H
