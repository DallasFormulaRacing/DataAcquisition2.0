/*
* ECU PE3 Frame: PE11
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

#ifndef ECU_PE3_FRAMES_FRAMEPE11_H
#define ECU_PE3_FRAMES_FRAMEPE11_H

#include "../frame_formats.hpp"

namespace sensor {

class FramePE11 : public FrameFormat2 {
public:
	FramePE11(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float PercentSlip()				{ return fields.at(0) * kResolutionPerBit; } // Resolution per Bit = 1
	float DrivenWheelRateOfChange()	{ return fields.at(1) * kResolutionPerBit; }
	float DesiredValue()			{ return fields.at(2) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

#endif // ECU_PE3_FRAMES_FRAMEPE11_H
