#include <gtest/gtest.h>

#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/frame_formats.hpp"

TEST(EcuFrameFieldsParsing, ParseBytePairOutput) {
    // Give dummy data and evaluate output
    
    // Check if >= than 15-bit minum and <= than 12-bit max
}

TEST(EcuFrameFieldsParsing, ParseBytePairInputOverflow) {
    // Give dummy data that causes overflow and evaluate output

    // Check if >= than 15-bit minum and <= than 12-bit max

    // Check that it is not the value if overflow was not handled
}

TEST(EcuFrameFieldsParsing, ParseFieldsOutput) {
    // Init vector with size of 2 or 3 (inclusively)

    // Check successful return status

    // Check that vector is unaltered
    
    // Check that all values are correct
}

TEST(EcuFrameFieldsParsing, ParseFieldsMaxFieldsRequested) {
    // Init vector with size of 4

    // Check successful return status

    // Check that vector is unaltered
    
    // Check that all values are correct
}

TEST(EcuFrameFieldsParsing, ParseFieldsMinFieldsRequested) {
    // Init vector with size of 1

    // Check successful return status

    // Check that vector is unaltered
    
    // Check that all values are correct
}

TEST(EcuFrameFieldsParsing, ParseFieldsNoFieldsRequested) {
    // Declare vector, do not initialize (size 0)

    // Check failed return status

    // Check that vector is unaltered
}

TEST(EcuFrameFieldsParsing, ParseFieldsExcessiveNumberOfFieldsRequested) {
    // Init vector with size greater than 4

    // Check failed return status

    // Check that vector is unaltered
    
    // Check that all values are of default value (zero)
}


TEST(EcuFrameFieldsParsing, ParseTypeBitUnkown) {

}

TEST(EcuFrameFieldsParsing, ParseTypeBitHigh) {
    
}

TEST(EcuFrameFieldsParsing, ParseTypeBitLow) {
    
}