/*
* Circular Queue Tests
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

// Unit Testing Framework
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../../DAQ_System/Program/Src/Application/CircularQueue/circular_queue.hpp"

namespace application {

TEST(CircularQueueTest, boop) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

} // namespace application