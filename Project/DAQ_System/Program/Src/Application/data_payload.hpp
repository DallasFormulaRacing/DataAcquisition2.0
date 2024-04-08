/*
* Data Payload
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef DATA_PAYLOAD_H
#define DATA_PAYLOAD_H

// Standard Libraries
#include <cstdint>
#include <array>


namespace application {

struct DataPayload {
    DataPayload() { }
    
    static constexpr uint8_t kNumOfLinPots = 4;
    std::array<float, kNumOfLinPots> linpot_displacement_inches_;

    static constexpr uint8_t kDegreesOfFreedom = 3;
    std::array<float, kDegreesOfFreedom> acceleration_;
    std::array<float, kDegreesOfFreedom> angular_velocity_;

    int16_t rpm_;
    float tps_;
    float fuel_open_time_;
    float ignition_angle_;
    float map_;
    float lambda_;
    float battery_voltage_;
    float air_temp_;
    float coolant_temp_;
};

} // namespace application

#endif // DATA_PAYLOAD_H
