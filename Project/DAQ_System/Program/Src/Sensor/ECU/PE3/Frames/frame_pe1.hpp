/*
* ECU PE3 Frame: PE1
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

#ifndef ECU_PE3_FRAMES_PE1_H
#define ECU_PE3_FRAMES_PE1_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE1 : public FrameFormat1 {
public:
	FramePE1(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	int16_t RevolutionsPerMinute()		{ return fields.at(0); } // Resolution per Bit = 1
	float ThrottlePosition()			{ return fields.at(1) * kResolutionPerBit; }
	float FuelOpenTime()				{ return fields.at(2) * kResolutionPerBit; }
	float IgnitionAngle()				{ return fields.at(3) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_PE1_H
