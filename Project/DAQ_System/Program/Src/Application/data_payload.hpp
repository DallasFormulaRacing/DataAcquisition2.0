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
#include <cinttypes>
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
    std::array<float, kNumOfLinPots> linpot_displacement_mm_{};

    static constexpr uint8_t kDegreesOfFreedom = 3;
    std::array<float, kDegreesOfFreedom> acceleration_{};
    std::array<float, kDegreesOfFreedom> angular_velocity_{};

    int16_t rpm_ = 0;
    float tps_ = 0.0f;
    float fuel_open_time_ = 0.0f;
    float ignition_angle_ = 0.0f;

    float barometer_ = 0.0f;
    float map_ = 0.0f;
    float lambda_ = 0.0f;

    static constexpr uint8_t kNumAnalogFields = 8;
	std::array<float, kNumAnalogFields> analog_inputs_{};

    float battery_voltage_ = 0.0f;
    float air_temp_ = 0.0f;
    float coolant_temp_ = 0.0f;

    int GetCsvFormatLength() {
    	return CsvString(NULL, 0);
	}

    void CsvFormat(char* buffer, int length) {
    	CsvString(buffer, length);
	}

    void Lock() { mutex_->Lock(); }

    void Unlock() { mutex_->Unlock(); }

    static constexpr char* kCsvHeaderRow = (char*)"Timestamp,"

    								   			  "Linpot1,"
    								   			  "Linpot2,"
    								   			  "Linpot3,"
    								   			  "Linpot4,"

    								   			  "AccX,"
    								   			  "AccY,"
    								   			  "AccZ,"

    								   			  "GyroX,"
    								   			  "GyroY,"
    								   			  "GyroZ,"

    								   			  "RPM,"
    								   			  "TPS,"
    								   			  "FuelOpenTime,"
    								   			  "IgnitionAngle,"

    								   			  "Barometer,"
    								   			  "MAP,"
    								   			  "Lambda,"

    								   			  "AnalogInput1,"
    								   			  "AnalogInput2,"
    								   			  "AnalogInput3,"
    								   			  "AnalogInput4,"
    								   			  "AnalogInput5,"
    								   			  "AnalogInput6,"
    								   			  "AnalogInput7,"
    								   			  "AnalogInput8,"

    								   			  "BatteryVoltage,"
    								   			  "AirTemp,"
    								   			  "CoolantTemp"
    								   			  "\n\0";

private:
    int CsvString(char* buffer, int length) {
    	return snprintf(buffer, length, kCsvFormatSpecifiers,
						timestamp_,

						linpot_displacement_mm_[0],
						linpot_displacement_mm_[1],
						linpot_displacement_mm_[2],
						linpot_displacement_mm_[3],

						acceleration_[0],
						acceleration_[1],
						acceleration_[2],

						angular_velocity_[0],
						angular_velocity_[1],
						angular_velocity_[2],

						rpm_,
						tps_,
						fuel_open_time_,
						ignition_angle_,

						barometer_,
						map_,
						lambda_,

						analog_inputs_[0],
						analog_inputs_[1],
						analog_inputs_[2],
						analog_inputs_[3],
						analog_inputs_[4],
						analog_inputs_[5],
						analog_inputs_[6],
						analog_inputs_[7],

						battery_voltage_,
						air_temp_,
						coolant_temp_);
    }

    std::shared_ptr<application::IMutex> mutex_;
    const char* kCsvFormatSpecifiers = "%f,"					// Timestamp
    								   "%f,%f,%f,%f,"			// Linpots
    								   "%f,%f,%f,"				// Accelerometer
    								   "%f,%f,%f,"				// Gyroscope
    								   "%" PRIx16 ",%f,%f,%f,"	// PE3 ECU, frame PE1
									   "%f,%f,%f,"				// PE3 ECU, frame PE2
									   "%f,%f,%f,%f,"			// PE3 ECU, frame PE3
									   "%f,%f,%f,%f,"			// PE3 ECU, frame PE4
									   "%f,%f,%f"				// PE3 ECU, frame PE6
									   "\n\0";					// New line and Null Terminator



};

} // namespace application

#endif // DATA_PAYLOAD_H
