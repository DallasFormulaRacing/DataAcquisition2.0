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

#ifndef ECU_PE3_FRAMES_PE10_H
#define ECU_PE3_FRAMES_PE10_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE10 : public FrameFormat3 {
public:
	FramePE10(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat3(rx_buffer) {}

	float PwmDutyCycle(uint8_t index) {
		if (index >= kNumOfFields) {
			return 0.0f;
		}
		
		return fields.at(index) * kResolutionPerBit;
	}

private:
	static constexpr float kResolutionPerBit = 0.5f;
};

}

#endif // ECU_PE3_FRAMES_PE10_H
