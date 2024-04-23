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
#include <stdio.h>
#include <cstdint>
#include <array>

// DFR Custom Dependencies
#include "Mutex/imutex.hpp"

namespace application {

struct DataPayload {
    DataPayload() { }
    
    DataPayload(std::shared_ptr<application::IMutex> mutex)
      : mutex_(mutex) { }

    float timestamp_ = 0.0f;

    static constexpr uint8_t kNumOfLinPots = 4;
    std::array<float, kNumOfLinPots> linpot_displacement_inches_{};

    static constexpr uint8_t kDegreesOfFreedom = 3;
    std::array<float, kDegreesOfFreedom> acceleration_{};
    std::array<float, kDegreesOfFreedom> angular_velocity_{};

    int16_t rpm_ = 0;
    float tps_ = 0.0f;
    float fuel_open_time_ = 0.0f;
    float ignition_angle_ = 0.0f;
    float map_ = 0.0f;
    float lambda_ = 0.0f;
    float battery_voltage_ = 0.0f;
    float air_temp_ = 0.0f;
    float coolant_temp_ = 0.0f;

    int GetCsvFormatLength() {
		return snprintf(NULL, 0,
				"%f,%f,%f,%f,%f\n",
				timestamp_,
				linpot_displacement_inches_[0],
				linpot_displacement_inches_[1],
				linpot_displacement_inches_[2],
				linpot_displacement_inches_[3]);
	}

    void CsvFormat(char* buffer, int length) {
		snprintf(buffer, length,
				"%f,%f,%f,%f,%f\n",
				timestamp_,
				linpot_displacement_inches_[0],
				linpot_displacement_inches_[1],
				linpot_displacement_inches_[2],
				linpot_displacement_inches_[3]);
	}

    void Lock() { mutex_->Lock(); }

    void Unlock() { mutex_->Unlock(); }

private:
    std::shared_ptr<application::IMutex> mutex_;


};

} // namespace application

#endif // DATA_PAYLOAD_H
