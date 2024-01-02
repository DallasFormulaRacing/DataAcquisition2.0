/*
* ECU PE3 Frame: PE5
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

#ifndef ECU_PE3_FRAMES_FRAMEPE5_H
#define ECU_PE3_FRAMES_FRAMEPE5_H

#include "../frame_formats.hpp"

namespace sensor {

class FramePE5 : public FrameFormat1 {
public:
	FramePE5(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat1(rx_buffer) {}

	float Frequency(uint8_t index) {
		return fields.at(index) * kResolutionPerBit;
	}

private:
	static constexpr float kResolutionPerBit = 0.2f;
};

}

#endif // ECU_PE3_FRAMES_FRAMEPE5_H
