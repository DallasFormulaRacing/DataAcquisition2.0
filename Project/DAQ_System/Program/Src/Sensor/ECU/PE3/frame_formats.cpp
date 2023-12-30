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


namespace sensor {


static int16_t ParseBytePair(const uint8_t &low, const uint8_t &high) {
	// TODO: Revisit the meaning and significants of 32767, 65536

	// Combine bytes
	int16_t field = (high << 8) + low;

	if (field > 32767) {
		field -= 65536;
	}
	return field;
}

// TODO: Document that the user is responsible for initializing the correct size per the number of byte pairs. This function does note
// alternate the size. The vector is expected to be initialized to the exact amount of byte pairs to collect as individual fields.
static void CollectFields(const uint8_t rx_buffer[kByteArrayMaxLength], std::vector<int16_t> &fields) {
	uint8_t max_index = fields.size() * 2;

	// TODO: validate `max_index` with `kByteArrayMaxLength`

	for (int i = 0; i < max_index; i++) {
		uint8_t low = rx_buffer[i];
		i++;
		uint8_t high = rx_buffer[i];

		fields.at(i/2) = ParseBytePair(low, high);
	}
}

// TODO: Document that TypeBit defaults to `kUnknown`.
static void CollectTypeBit(const uint8_t rx_buffer[kByteArrayMaxLength], TypeBit &type) {
	uint8_t type_bit = rx_buffer[kByteArrayMaxLength - 1];

	// TODO: Verify how the type bit is stored, and what value to expect, via logic analyzer
	if (type_bit == 1) {
		type = TypeBit::kHigh;
	} else if (type_bit == 0) {
		type = TypeBit::kLow;
	}
}

}
