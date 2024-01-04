/*
* ECU PE3 Frame Parsing Unit Tests
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

// Standard Libraries
#include <cmath>
#include <vector>

// Unit Testing Framework
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// DFR Custom Dependencies
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/frame_parsing.hpp"

namespace sensor {

/*****************Byte Parsing Tests******************/
TEST(Pe3EcuByteParsing, ParseBytePairOutput) {
    static constexpr uint8_t kLowByte = 0x1E;
    static constexpr uint8_t kHighByte = 0x05;
    int16_t field_result = ParseBytePair(kLowByte, kHighByte);

    static constexpr int16_t kExpectedResult = 1310;
    EXPECT_EQ(field_result, kExpectedResult);
}

TEST(Pe3EcuByteParsing, ParseBytePairInputOverflow) {
    static constexpr uint8_t kLowByte = UINT8_MAX; // 255
    static constexpr uint8_t kHighByte = 150;
    int16_t field_result = ParseBytePair(kLowByte, kHighByte);

    // Check that the result is within proper range
    static constexpr int16_t kMaximumValue = INT16_MAX; // 32,767
    static constexpr int16_t kMinimumValue = INT16_MIN; // -32,768
    EXPECT_LT(field_result, kMaximumValue + 1);
    EXPECT_GT(field_result, kMinimumValue - 1);

    static constexpr int16_t kExpectedResult = -26881;
    EXPECT_EQ(field_result, kExpectedResult);
}

/******************Field Parsing Tests******************/
class Pe3EcuFrameFieldParsingFixture : public testing::Test {
protected:
    std::vector<int16_t> fields;

    // Sample data
    static constexpr uint8_t kByteArrayMaxLength = 8;
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x05,
                                                0xBB, 0x05,
                                                0x8F, 0x03,
                                                0x00, 0xFF  };
    
    // Expected output
    const int16_t kExpectedField1 = 1470;
    const int16_t kExpectedField2 = 1467;
    const int16_t kExpectedField3 = 911;
    const int16_t kExpectedField4 = 65280;
};

TEST_F(Pe3EcuFrameFieldParsingFixture, ParseFieldsOutput) {
    static constexpr uint8_t kNumOfFields = 3;
    fields = std::vector<int16_t>(kNumOfFields);

    EXPECT_TRUE(ParseFields(rx_buffer, fields)) << "Failed to parse: vector is too large.";
    EXPECT_EQ(fields.size(), kNumOfFields);
    EXPECT_THAT(fields, testing::ElementsAre(kExpectedField1, kExpectedField2, kExpectedField3));
}

TEST_F(Pe3EcuFrameFieldParsingFixture, MaximumFieldsRequested) {
    static constexpr uint8_t kNumOfFields = 4;
    fields = std::vector<int16_t>(kNumOfFields);

    EXPECT_TRUE(ParseFields(rx_buffer, fields)) << "Failed to parse: vector is too large.";
    EXPECT_EQ(fields.size(), kNumOfFields);
    EXPECT_THAT(fields, testing::ElementsAre(kExpectedField1, kExpectedField2, kExpectedField3, kExpectedField4));
}

TEST_F(Pe3EcuFrameFieldParsingFixture, MinimumFieldsRequested) {
    static constexpr uint8_t kNumOfFields = 1;
    fields = std::vector<int16_t>(kNumOfFields);

    EXPECT_TRUE(ParseFields(rx_buffer, fields)) << "Failed to parse: vector is too large.";
    EXPECT_EQ(fields.size(), kNumOfFields);
    EXPECT_THAT(fields, testing::ElementsAre(kExpectedField1));
}

TEST_F(Pe3EcuFrameFieldParsingFixture, NoFieldsRequested) {
    // Using an un-initialized, empty vector
    EXPECT_FALSE(ParseFields(rx_buffer, fields));
    EXPECT_EQ(fields.size(), 0);
}

TEST_F(Pe3EcuFrameFieldParsingFixture, ExcessiveFieldsRequested) {
    static constexpr uint8_t kNumOfFields = 5;
    fields = std::vector<int16_t>(kNumOfFields);

    EXPECT_FALSE(ParseFields(rx_buffer, fields));
}

TEST_F(Pe3EcuFrameFieldParsingFixture, ParseTypeBitUnkown) {
    rx_buffer[7] = 0xA0;
    EXPECT_EQ(ParseTypeBit(rx_buffer), TypeBit::kUnknown);
}

TEST_F(Pe3EcuFrameFieldParsingFixture, ParseTypeBitHigh) {
    EXPECT_EQ(ParseTypeBit(rx_buffer), TypeBit::kHigh);
}

TEST_F(Pe3EcuFrameFieldParsingFixture, ParseTypeBitLow) {
    rx_buffer[7] = 0;
    EXPECT_EQ(ParseTypeBit(rx_buffer), TypeBit::kLow);
}

}