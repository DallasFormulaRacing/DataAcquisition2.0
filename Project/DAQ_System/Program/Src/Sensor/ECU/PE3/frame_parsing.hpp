/*
* ECU PE3 Frame Format
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

#ifndef ECU_PE3_FRAME_PARSING_H
#define ECU_PE3_FRAME_PARSING_H

// Standard Libraries
#include <cstdint>
#include <vector>
#include <cmath>

namespace sensor {

static constexpr uint8_t kByteArrayMaxLength = 8;

enum class TypeBit : uint8_t {
	kUnknown,
	kHigh,
	kLow,
};

// Parsing helper functions
int16_t ParseBytePair(const uint8_t &low, const uint8_t &high);
bool ParseFields(const uint8_t rx_buffer[kByteArrayMaxLength], std::vector<int16_t> &fields);
TypeBit ParseTypeBit(const uint8_t rx_buffer[kByteArrayMaxLength]);

/*
Data type  | DLC | Data Breakdown | Length
-------------------------------------------
	 1	   |  8  |     1 - 2      | 2 bytes
 	  	   |	 |     3 - 4 	  | 2 bytes
 	  	   |	 |     5 - 6 	  | 2 bytes
 	  	   |	 |     7 - 8 	  | 2 bytes
*/
struct FrameFormat1 {
	FrameFormat1(uint8_t rx_buffer[kByteArrayMaxLength]) {
		fields = std::vector<int16_t>(kNumOfFields);
		ParseFields(rx_buffer, fields);
	}

	static constexpr uint8_t kNumOfFields = 4;
	std::vector<int16_t> fields;
};



/*
Data type  | DLC | Data Breakdown | Length
-------------------------------------------
	 2	   |  6  |     1 - 2      | 2 bytes
 	  	   |	 |     3 - 4 	  | 2 bytes
 	  	   |	 |     5 - 6 	  | 2 bytes
*/
struct FrameFormat2 {
	FrameFormat2(uint8_t rx_buffer[kByteArrayMaxLength]) {
		fields = std::vector<int16_t>(kNumOfFields);
		ParseFields(rx_buffer, fields);
	}

	static constexpr uint8_t kNumOfFields = 3;
	std::vector<int16_t> fields;
};


/*
Data type | DLC | Data Breakdown | Length
-------------------------------------------
	 3	  |  8  | 	     1       | 1 byte
	      |     |        2		 | 1 byte
	      |     |        3		 | 1 byte
	      |     |        4		 | 1 byte
	      |     |        5		 | 1 byte
	      |     |        6		 | 1 byte
	      |     |        7		 | 1 byte
	      |     |        8		 | 1 byte
*/
struct FrameFormat3 {
	FrameFormat3(uint8_t rx_buffer[kByteArrayMaxLength]) {
		for (int i = 0; i < kByteArrayMaxLength; i++) {
			fields.push_back(rx_buffer[i]);
		}
	}

	static constexpr uint8_t kNumOfFields = kByteArrayMaxLength;
	std::vector<int16_t> fields;
};

/*
Data type | DLC | Data Breakdown | Length
-------------------------------------------
     4    |  7  |	  1 - 2 	 | 2 bytes
		  |     |     3 - 4		 | 2 bytes
		  |     |     5 - 6		 | 2 bytes
		  |     |     	7	 	 | 1 bytes
*/
struct FrameFormat4 {
	FrameFormat4(uint8_t rx_buffer[kByteArrayMaxLength]) {
		fields = std::vector<int16_t>(kNumOfFields);
		ParseFields(rx_buffer, fields);
		type = ParseTypeBit(rx_buffer);
	}

	static constexpr uint8_t kNumOfFields = 3;
	std::vector<int16_t> fields;
	TypeBit type = TypeBit::kUnknown;
};

/*
Data type | DLC | Data Breakdown | Length
-------------------------------------------
	 5    |  8  | 	  1 - 2      | 2 bytes
		  |     |     3 - 4      | 2 bytes
		  |		|       5        | 1 byte
		  |		|		6		 | 1 byte
		  |		|       7 		 | 1 byte
		  |		|		8		 | 1 byte
*/
struct FrameFormat5 {
	FrameFormat5(uint8_t rx_buffer[kByteArrayMaxLength]) {
		fields = std::vector<int16_t>(kNumOfFields);
		ParseFields(rx_buffer, fields);
	}

	static constexpr uint8_t kNumOfFields = 2; // The remaining fields are "don't cares"
	std::vector<int16_t> fields;
};

}

#endif // ECU_PE3_FRAME_PARSING_H
