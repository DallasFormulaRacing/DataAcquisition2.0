#include <gtest/gtest.h>

#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/frame_formats.hpp"

// Here, we are only testing the formats

TEST(EcuFrameFormats, FrameFormat1Creation) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat1 format(rx_buffer);

  // Verify that vector values are not zero (default value)
  
  // Passing dummy data and validating that the output is correct

  // Are 4 fields created? No more, no less. Vectors are un-altered.

  // Should fail: Passing an rx_buffer of size other than 8.
  // This probably simply won't compile
}

TEST(EcuFrameFormats, FrameFormat2Creation) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

  // Verify that vector values are not zero (default value)
  
  // Passing dummy data and validating that the output is correct

  // Are 3 fields created? No more, no less. Vectors are un-altered.

  // Should fail: Passing an rx_buffer of size other than 8.
  // This probably simply won't compile
}

TEST(EcuFrameFormats, FrameFormat3Creation) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

  // Verify that vector values are not zero (default value)
  
  // Passing dummy data and validating that the output is correct

  // Are 8 fields created? No more, no less. Vectors are un-altered.

  // Should fail: Passing an rx_buffer of size other than 8.
  // This probably simply won't compile
}

TEST(EcuFrameFormats, FrameFormat4Creation) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

  // Verify that vector values are not zero (default value)
  
  // Passing dummy data and validating that the output is correct

  // Are 3 fields created? No more, no less. Vectors are un-altered.

  // Should fail: Passing an rx_buffer of size other than 8.
  // This probably simply won't compile
}

TEST(EcuFrameFormats, FrameFormat4TypeBitIsUnkown) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

}

TEST(EcuFrameFormats, FrameFormat4TypeBitIsHigh) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

}

TEST(EcuFrameFormats, FrameFormat4TypeBitIsLow) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

}

TEST(EcuFrameFormats, FrameFormat5Creation) {
  uint8_t rx_buffer[8];
  sensor::FrameFormat2 format(rx_buffer);

  // Verify that vector values are not zero (default value)
  
  // Passing dummy data and validating that the output is correct

  // Are 2 fields created? No more, no less. Vectors are un-altered.

  // Should fail: Passing an rx_buffer of size other than 8.
  // This probably simply won't compile
}
