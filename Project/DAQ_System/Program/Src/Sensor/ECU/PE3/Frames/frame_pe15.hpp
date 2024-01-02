/*
* ECU PE3 Frame: PE15
* Author:   William Lim
*           Cristian Cruz
*
* Email:    wll180000@utdallas.edu
*           cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef ECU_PE3_FRAMES_FRAMEPE15_H
#define ECU_PE3_FRAMES_FRAMEPE15_H

#include "../frame_formats.hpp"

namespace sensor {

class FramePE15 : public FrameFormat1 {
public:
	FramePE15(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float FuelCompensationBarometer()					{ return fields.at(0) * kResolutionPerBit; }
	float FuelCompensationManifoldAbsolutePressure()	{ return fields.at(1) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_FRAMEPE15_H
