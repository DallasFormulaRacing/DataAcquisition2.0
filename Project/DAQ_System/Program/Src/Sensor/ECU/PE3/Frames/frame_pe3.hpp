/*
* ECU PE3 Frame: PE3
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

#ifndef ECU_PE3_FRAMES_FRAMEPE3_H
#define ECU_PE3_FRAMES_FRAMEPE3_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE3 : public FrameFormat1 {
public:
	FramePE3(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float AnalogInputVoltage(uint8_t index) {
		if (index >= kNumOfFields) {
			return 0.0f;
		}

		return fields.at(index) * kResolutionPerBit;
	}

private:
	static constexpr float kResolutionPerBit = 0.001f;
};

}

#endif // ECU_PE3_FRAMES_FRAMEPE3_H
