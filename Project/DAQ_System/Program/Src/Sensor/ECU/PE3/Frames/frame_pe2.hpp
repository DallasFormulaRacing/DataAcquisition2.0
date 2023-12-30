/*
* ECU PE3 Frame: PE2
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

#ifndef ECU_PE3_FRAMES_FRAMEPE2_H
#define ECU_PE3_FRAMES_FRAMEPE2_H

#include "../frame_formats.hpp"

namespace sensor {

enum class PressureType : uint8_t {
	kUnknown,
	kKiloPascal,
	kPoundsPerSquareInch,
};

class FramePE2 : public FrameFormat4 {
public:
	FramePE2(uint8_t rx_buffer[kByteArrayMaxLength])
	  : FrameFormat4(rx_buffer) {}

	float Barometer()					{ return fields.at(0) * kResolutionPerBit; }
	float ManifoldAbsolutePressure()	{ return fields.at(1) * kResolutionPerBit; }
	float Lambda()						{ return fields.at(2) * kResolutionPerBit; }

	PressureType PressureType() {
		switch(type) {
			case(TypeBit::kUnknown): return PressureType::kUnknown;
			case(TypeBit::kHigh): return PressureType::kKiloPascal;
			case(TypeBit::kLow): return PressureType::kPoundsPerSquareInch;
		}
	}

private:
	static constexpr float kResolutionPerBit = 0.01f;
};

#endif // ECU_PE3_FRAMES_FRAMEPE2_H
