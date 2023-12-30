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

#ifndef ECU_PE3_FRAME_FORMATS_H
#define ECU_PE3_FRAME_FORMATS_H

#include <stdint.h>
#include <vector>

namespace sensor {

static constexpr uint8_t kByteArrayMaxLength = 8;

static void CollectFields(const uint8_t rx_buffer[kByteArrayMaxLength], const uint8_t &kNumOfBytePairs, std::vector<int16_t> &fields);

enum class TypeBit : uint8_t {
	kUnknown,
	kHigh,
	kLow,
};

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
		CollectFields(rx_buffer, kNumOfFields, fields);
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
		CollectFields(rx_buffer, kNumOfFields, fields);
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
		fields = std::vector<int16_t>(kNumOfFields);
		CollectFields(rx_buffer, kNumOfFields, fields);
	}

	static constexpr uint8_t kNumOfFields = 8;
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
		CollectFields(rx_buffer, kNumOfFields, fields);
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
		CollectFields(rx_buffer, kNumOfBytePairs, fields);
	}

	static constexpr uint8_t kNumOfFields = 2; // The remaining fields are "don't cares"
	std::vector<int16_t> fields;

private:
	static constexpr uint8_t kNumOfBytePairs = 2;
};

}

#endif // ECU_PE3_FRAME_FORMATS_H
