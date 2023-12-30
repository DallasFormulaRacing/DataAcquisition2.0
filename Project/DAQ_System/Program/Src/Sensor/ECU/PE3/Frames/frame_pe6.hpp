/*
* ECU PE3 Frame: PE6
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

#ifndef ECU_PE3_FRAMES_FRAMEPE6_H
#define ECU_PE3_FRAMES_FRAMEPE6_H

#include "../frame_formats.hpp"

namespace sensor {

enum class TemperatureType : uint8_t {
	kUnknown,
	kCelsius,
	kFarenheit,
};

class FramePE6 : public FrameFormat4 {
public:
	FramePE6(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat4(rx_buffer) {}

	float BatteryVoltage()		{ return fields.at(0) * kBatteryResolutionPerBit; }
	float AirTemperature()		{ return fields.at(1) * kTempResolutionPerBit; }
	float CoolantTemperature()	{ return fields.at(2) * kTempResolutionPerBit; }

	TemperatureType TemperatureType() {
		switch(type) {
			case(TypeBit::kUnknown): return TemperatureType::kUnknown;
			case(TypeBit::kHigh): return TemperatureType::kCelsius;
			case(TypeBit::kLow): return TemperatureType::kFarenheit;
		}
	}

private:
	static constexpr float kBatteryResolutionPerBit = 0.01f;
	static constexpr float kTempResolutionPerBit = 0.1f;
};

#endif // ECU_PE3_FRAMES_FRAMEPE6_H
