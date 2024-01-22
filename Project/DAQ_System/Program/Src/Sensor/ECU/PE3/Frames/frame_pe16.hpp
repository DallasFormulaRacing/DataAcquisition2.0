/*
* ECU PE3 Frame: PE16
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

#ifndef ECU_PE3_FRAMES_PE16_H
#define ECU_PE3_FRAMES_PE16_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE16 : public FrameFormat1 {
public:
	FramePE16(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float IgnitionCompensationAirTemperature()				{ return fields.at(0) * kResolutionPerBit; }
	float IgnitionCompensationCoolantTemperature()			{ return fields.at(1) * kResolutionPerBit; }
	float IgnitionCompensationBarometer()					{ return fields.at(2) * kResolutionPerBit; }
	float IgnitionCompensationManifoldAbsolutePressure()	{ return fields.at(3) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_PE16_H
