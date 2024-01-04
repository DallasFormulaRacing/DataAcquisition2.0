/*
* ECU PE3 Frame: PE13
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

#ifndef ECU_PE3_FRAMES_FRAMEPE13_H
#define ECU_PE3_FRAMES_FRAMEPE13_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE13 : public FrameFormat1 {
public:
	FramePE13(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float DrivenWheelSpeed(uint8_t index) {
		if (index >= 2) {
			return 0.0f;
		}

		return fields.at(index) * kResolutionPerBit;
	}

	float NonDrivenWheelSpeed(uint8_t index) {
		index += 2;
		if (index <= 1 || index >= kNumOfFields) {
			return 0.0f;
		}

		return fields.at(index) * kResolutionPerBit;
	}

private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_FRAMEPE13_H
