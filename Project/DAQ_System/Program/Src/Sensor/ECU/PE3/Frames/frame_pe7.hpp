/*
* ECU PE3 Frame: PE7
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

#ifndef ECU_PE3_FRAMES_PE7_H
#define ECU_PE3_FRAMES_PE7_H

#include "../frame_parsing.hpp"

namespace sensor {

class FramePE7 : public FrameFormat5 {
public:
	FramePE7(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat5(rx_buffer) {}

	float AnalogInputThermistorVoltage(uint8_t index) {
		if (index >= kNumOfFields) {
			return 0.0f;
		}

		return fields.at(index) * kResolutionPerBit;
	}

	// TODO: Determine Celsius or Farenheit
	// Does this rely on Frame PE6's TemperatureType bit?
private:
	static constexpr float kResolutionPerBit = 0.1f;
};

}

#endif // ECU_PE3_FRAMES_PE7_H
