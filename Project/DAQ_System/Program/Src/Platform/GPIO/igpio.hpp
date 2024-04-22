/*
* GPIO Abstract Interface
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IGPIO_H
#define IGPIO_H

// Standard Libraries
#include <cstdint>

namespace platform {

class IGpio {
public:
	virtual ~IGpio() = default;

	virtual bool Read() = 0;

	virtual void Write(bool signal) = 0;

	virtual bool ToggleDetected() = 0;
};

} // namespace platform

#endif // IGPIO_H
