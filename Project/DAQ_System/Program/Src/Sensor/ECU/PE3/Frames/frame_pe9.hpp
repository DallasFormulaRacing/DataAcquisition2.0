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

#ifndef ECU_PE3_FRAMES_FRAMEPE9_H
#define ECU_PE3_FRAMES_FRAMEPE9_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE9 : public FrameFormat2 {
public:
	FramePE9(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat2(rx_buffer) {}

	float LambdaMeasured(uint8_t index) {
		if (index >= 2) {
			return 0.0f;
		}
		
		return fields.at(index) * kResolutionPerBit;
	}

	float TargetLambda() { return fields.at(2) * kResolutionPerBit; }

private:
	static constexpr float kResolutionPerBit = 0.01f;
};

}

#endif // ECU_PE3_FRAMES_FRAMEPE9_H
