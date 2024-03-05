/*
* ECU PE3 Frame Formats
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

#include "frame_parsing.hpp"

namespace sensor {

std::vector<int16_t> FrameFormat1::fields;
std::vector<int16_t> FrameFormat2::fields;
std::vector<int16_t> FrameFormat3::fields;
std::vector<int16_t> FrameFormat4::fields;
std::vector<int16_t> FrameFormat5::fields;

int16_t ParseBytePair(const uint8_t &low, const uint8_t &high) {
	static constexpr int k15BitDataTotalRange = pow(2, 15) * 2; // 65,536

	// Combine bytes
	int32_t field = (high << 8) + low;

	// Check and account for 15-bit data overflow
	if (field > INT16_MAX) {
		field -= k15BitDataTotalRange;
	}

	// Provide only the 16 bits
	return field & 0xFFFF;
}

bool ParseFields(const uint8_t rx_buffer[kByteArrayMaxLength], std::vector<int16_t> &fields) {
	uint8_t max_index = (fields.size() * 2) - 1;
	if (max_index >= kByteArrayMaxLength) {
		return false;
	}

	for (int i = 0; i <= max_index; i++) {
		uint8_t low = rx_buffer[i];
		i++;
		uint8_t high = rx_buffer[i];

		fields.at(i/2) = ParseBytePair(low, high);
	}

	return true;
}

TypeBit ParseTypeBit(const uint8_t rx_buffer[kByteArrayMaxLength]) {
	static constexpr uint8_t kHigh = 0x01;
	static constexpr uint8_t kLow = 0;

	uint8_t type_bit = rx_buffer[kByteArrayMaxLength - 2];

	if (type_bit == kHigh) {
		return TypeBit::kHigh;
	} else if (type_bit == kLow) {
		return TypeBit::kLow;
	}

	return TypeBit::kUnknown;
}

} // namespace sensor
