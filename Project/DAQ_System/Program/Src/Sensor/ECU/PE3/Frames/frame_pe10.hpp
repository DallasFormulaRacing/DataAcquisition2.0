/*
* ECU PE3 Frame: PE10
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

#ifndef ECU_PE3_FRAMES_FRAMEPE10_H
#define ECU_PE3_FRAMES_FRAMEPE10_H

#include "../frame_formats.hpp"

namespace sensor {

class FramePE10 : public FrameFormat3 {
public:
	FramePE10(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float PulseWidthModulationDutyCycle(uint8_t index) {
		return fields.at(index) * kResolutionPerBit;
	}

private:
	static constexpr float kResolutionPerBit = 0.5f;
};

#endif // ECU_PE3_FRAMES_FRAMEPE10_H
