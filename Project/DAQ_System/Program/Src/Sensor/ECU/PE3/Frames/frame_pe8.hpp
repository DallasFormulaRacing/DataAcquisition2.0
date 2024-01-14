/*
* ECU PE3 Frame: PE8
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

#ifndef ECU_PE3_FRAMES_PE8_H
#define ECU_PE3_FRAMES_PE8_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE8 : public FrameFormat1 {
public:
	FramePE8(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float RevolutionsPerMinuteRate()		{ return fields.at(0); }
	float ThrottlePositionPercentageRate()	{ return fields.at(1); }
	float ManifoldAbsolutePressureRate()	{ return fields.at(2); }
	float MassAirFlowLoadRate()				{ return fields.at(3) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_PE8_H
