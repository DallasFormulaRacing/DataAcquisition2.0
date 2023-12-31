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

#include "frame_formats.hpp"

#include <cmath>

namespace sensor {

// TODO: Document the meaning and significance of 32767, 65536
// Source: https://www.quora.com/Why-is-32767-the-highest-value-number-that-can-be-made-into-binary-code-How-can-I-represent-any-higher-values-using-binary-code
// The variable `field` is combining bytes into a form of Two's Complements
// i.e., the data is actually 15 bits: 2^15 - 1 = 32,767 for a highest possible value
// Overflow must be handled for 15 bits, not 16.
// `field` is a signed variable, so it's range is (from -32,767 to +32,767) = (32,767 * 2) = 65,536 possible values
// Subtracting by 65,536 resets the variable to the bottom
static int16_t ParseBytePair(const uint8_t &low, const uint8_t &high) {
	static constexpr int k15BitTotalRange = pow(2, 15) * 2;

	// Combine bytes
	int16_t field = (high << 8) + low;

	// Check and account for overflow
	if (field > INT16_MAX) {
		field -= k15BitTotalRange;
	}

	return field;
}

// TODO: Document that the user is responsible for initializing the correct size per the number of byte pairs. This function does not
// alternate the size. The vector is expected to be initialized to the exact amount of byte pairs to collect as individual fields.
//
// Returns false when failed to parse due to incorrect indexing
static bool CollectFields(const uint8_t rx_buffer[kByteArrayMaxLength], std::vector<int16_t> &fields) {
	uint8_t max_index = fields.size() * 2;
	if (max_index >= kByteArrayMaxLength) {
		return false;
	}

	for (int i = 0; i < max_index; i++) {
		uint8_t low = rx_buffer[i];
		i++;
		uint8_t high = rx_buffer[i];

		fields.at(i/2) = ParseBytePair(low, high);
	}

	return true;
}


// TODO: Document that TypeBit defaults to `kUnknown`.
//
// ECU datasheet describes one "bit", but it is actually a whole byte of 1s (0xFF).
// This may be a "bit" in the sense that the byte will only have two possible values (hence, binary bit).
static TypeBit CollectTypeBit(const uint8_t rx_buffer[kByteArrayMaxLength]) {
	static constexpr uint8_t kHigh = 0xFF;
	static constexpr uint8_t kLow = 0;

	uint8_t type_bit = rx_buffer[kByteArrayMaxLength - 1];

	if (type_bit == kHigh) {
		return TypeBit::kHigh;
	} else if (type_bit == kLow) {
		return TypeBit::kLow;
	}

	return TypeBit::kUnknown;
}

}
