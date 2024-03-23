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

// Standard Libraries
#include <vector>
#include <string>

// DFR Custom Dependencies
#include "../../../DAQ_System/Program/Src/Application/circular_queue.hpp"


namespace application {

TEST(CircularQueueTest, AddingAndRemoving) {
    static constexpr uint8_t kSize = 5;
    CircularQueue<std::string> q(kSize);

    std::vector<std::string> sample_data{ "Hello", "World!", ":)" };

    // Insert all data
    for(std::string str : sample_data) {
        q.Enqueue(str);
    }

    // Remove all data
    EXPECT_FALSE(q.IsEmpty());
    for(std::string str : sample_data) {
        EXPECT_EQ(str, q.Dequeue());
    }

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
    EXPECT_EQ("", q.Dequeue());
}

TEST(CircularQueueTest, DequeueWhenEmpty) {
    static constexpr uint8_t kSize = 5;
    CircularQueue<float> q(kSize);

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
    EXPECT_EQ(0.0f, q.Dequeue());
}

TEST(CircularQueueTest, InsertUntilFull) {
    static constexpr uint8_t kSize = 3;
    CircularQueue<float> q(kSize);

    std::vector<float> sample_data{ 11.2f, 7.9f, 52.1f};
    for(float num : sample_data) {
        q.Enqueue(num);
    }

    EXPECT_TRUE(q.IsFull());
    EXPECT_FALSE(q.IsEmpty());
}

TEST(CircularQueueTest, InsertAndOverwriteWhenFull) {
    static constexpr uint8_t kSize = 5;
    CircularQueue<int> q(kSize);

    std::vector<int> sample_data{ 19, 34, 56, 98, 67};
    for(uint8_t num : sample_data) {
        q.Enqueue(num);
    }

    int overwriting_num1 = 2;
    int overwriting_num2 = 10;

    q.Enqueue(overwriting_num1);
    q.Enqueue(overwriting_num2);

    int result = q.Dequeue();
    EXPECT_NE(sample_data.at(0), result); // Overwritten and lost
    EXPECT_NE(sample_data.at(1), result); // Overwritten and lost
    EXPECT_EQ(sample_data.at(2), result); // The currently oldest entry

    result = q.Dequeue();
    EXPECT_EQ(sample_data.at(3), result);

    EXPECT_EQ(sample_data.at(4), q.Dequeue());
    EXPECT_EQ(overwriting_num1, q.Dequeue());
    EXPECT_EQ(overwriting_num2, q.Dequeue());
}

} // namespace application