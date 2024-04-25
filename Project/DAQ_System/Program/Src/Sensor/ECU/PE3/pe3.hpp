/*
* ECU PE3
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

#ifndef ECU_PE3_PE3_H
#define ECU_PE3_PE3_H

// Standard Libraries
#include <inttypes.h>

#include <vector>
#include <array>
#include <memory>

// DFR Custom Dependencies
#include "iecu.hpp"
#include "../../../Platform/CAN/Interfaces/ican.hpp"

// Internal ECU Messages
#include "Frames/frame_pe1.hpp"
#include "Frames/frame_pe2.hpp"
#include "Frames/frame_pe3.hpp"
#include "Frames/frame_pe4.hpp"
#include "Frames/frame_pe5.hpp"
#include "Frames/frame_pe6.hpp"
#include "Frames/frame_pe7.hpp"
#include "Frames/frame_pe8.hpp"
#include "Frames/frame_pe9.hpp"
#include "Frames/frame_pe10.hpp"
#include "Frames/frame_pe11.hpp"
#include "Frames/frame_pe12.hpp"
#include "Frames/frame_pe13.hpp"
#include "Frames/frame_pe14.hpp"
#include "Frames/frame_pe15.hpp"
#include "Frames/frame_pe16.hpp"

namespace sensor {

#define FramePe1Id		0x0CFFF048
#define FramePe2Id		0x0CFFF148
#define FramePe3Id		0x0CFFF248
#define FramePe4Id		0x0CFFF348
#define FramePe5Id		0x0CFFF448
#define FramePe6Id		0x0CFFF548
#define FramePe7Id		0x0CFFF648
#define FramePe8Id		0x0CFFF748
#define FramePe9Id		0x0CFFF848
#define FramePe10Id		0x0CFFF948
#define FramePe11Id		0x0CFFFA48
#define FramePe12Id		0x0CFFFB48
#define FramePe13Id		0x0CFFFC48
#define FramePe14Id		0x0CFFFD48
#define FramePe15Id		0x0CFFFE48
#define FramePe16Id		0x0CFFD048

class Pe3 : public IEcu {
public:
	/// @param can_bus An implementation of a CAN peripheral following the
	/// @ref platform.ICan abstract interface. This ECU requires it for
	/// the following functionalities:
	/// - a flag indicating for checking when a new CAN message has arrived.
	/// - a method to clear the flag after processing.
	/// - the CAN ID of the latest message received.
	/// - the message's data payload in the form of an array of 8 bytes.
	Pe3(std::shared_ptr<platform::ICan> can_bus);

	virtual ~Pe3();

	/// Provides CAN IDs currently enabled to read from.
	/// @return A reference to a read-only list of the enabled CAN IDs.
	const std::vector<uint32_t>& CanIdList() override;

	/// @return The CAN ID of the latest message received.
	uint32_t LatestCanId() override;

	/// @return A flag indicating whether a new message has arrived. 
	bool NewMessageArrived() override;

	/// Processes the current message and then clears the `NewMessageArrived()` flag.
	void Update() override;

	/// @return The number of Revolutions per Minute.
	int16_t Rpm() override;

	/// @return The Throttle Position expressed as a percentage.
	float Tps() override;

	/// @return The time duration of how long the fuel injector remains
	/// open in milliseconds.
	float FuelOpenTime() override;

	/// @return The ignition angle in degrees.
	float IgnitionAngle() override;

	/// @return Pressure in PSI or Kilo-Pascal.
	/// To determine the unit of measurement being used, see `PressureUnit()`.
	float BarometerPressure() override;

	/// @return Manifold Air Pressure in PSI or Kilo-Pascal.
	/// To determine the unit of measurement being used, see `PressureUnit()`.
	float Map() override;

	/// @return The level of oxygen in the exhaust.
	float Lambda() override;

	/// Provides access to the analog input measurements.
	/// @param index The selected channel to read from. 8 channels are provided
	/// and they are zero-indexed.
	/// @return The voltage measurement at an analog input channel.
	float AnalogInputVoltage(uint8_t index) override;

	/// @return The battery voltage in Volts.
	float BatteryVoltage() override;

	/// @return The air temperature in Celsius or Fahrenheit .
	/// To determine the unit of measurement being used, see `TemperatureUnit()`.
	float AirTemperature() override;

	/// @return The air temperature in Celsius or Fahrenheit .
	/// To determine the unit of measurement being used, see `TemperatureUnit()`.
	float CoolantTemperature() override;

	/// @return Indicates whether the pressure unit is PSI or Kilo-Pascal.
	PressureType PressureUnit();

	/// @return Indicates whether the temperature unit is Celsius or Fahrenheit.
	TemperatureType TemperatureUnit();

	/// Provides access to the frequency channels.
	/// @param index The selected channel to read from. 2 channels are provided
	/// and they are zero-indexed.
	/// @return A channel's frequency in hertz.
	float FrequencyHertz(uint8_t index);

	/// Provides access to the analog input measurements dedicated to thermistors.
	/// @param index The selected channel to read from. 4 channels are provided
	/// and they are zero-indexed.
	/// @return The voltage measurement at an analog input channel.
	float AnalogInputThermistorVoltage(uint8_t index);

	/// @return RPM per second.
	float RpmRate();

	/// @return Throttle position percentage per second.
	float TpsPercentageRate();

	/// @return Pressure rate in PSI per second or Kilo-Pascal per second.
	float MapRate();

	/// @return Load rate in Gs per revolution per second.
	float MassAirFlowLoadRate();

	/// Provides access to the <em> Lambda Measured </em> channels.
	/// @param index The selected channel to read from. 2 channels are provided
	/// and they are zero-indexed.
	/// @return The lambda measured at a channel.
	float LambdaMeasured(uint8_t index);

	/// Provides access to the PWM channels.
	/// @param index The selected channel to read from. 8 channels are provided
	/// and they are zero-indexed.
	/// @return The duty cycle percentage.  
	float PwmDutyCycle(uint8_t index);

	/// @return Slip in percentage.
	float PercentSlip();

	/// @return The driven wheel rate of change in feet per second per second.
	float DrivenWheelRateOfChange();

	float DesiredValue();

	/// @return Speed in feet per second.
	float DrivenAverageWheelSpeed();

	/// @return Speed in feet per second.
	float NonDrivenAverageWheelSpeed();

	/// @return Ignition compensation in degrees.
	float IgnitionCompensation();

	float IgnitionCutPercentage();

	/// Provides access to the <em>Driven Wheel Speed</em> channels.
	/// @param index The selected channel to read from. 2 channels are provided
	/// and they are zero-indexed.
	/// @return Speed in feet per second.
	float DrivenWheelSpeed(uint8_t index);

	// Provides access to the <em>Non Driven Wheel Speed</em> channels.
	/// @param index The selected channel to read from. 2 channels are provided
	/// and they are zero-indexed.
	/// @return Speed in feet per second.
	float NonDrivenWheelSpeed(uint8_t index);

	/// @return Fuel compensation in percentage.
	float FuelCompensationAcceleration();

	/// @return Fuel compensation in percentage.
	float FuelCompensationStarting();

	/// @return Fuel compensation in percentage.
	float FuelCompensationAirTemperature();

	/// @return Fuel compensation in percentage.
	float FuelCompensationCoolantTemperature();

	/// @return Fuel compensation in percentage.
	float FuelCompensationBarometer();

	/// @return Fuel compensation in percentage.
	float FuelCompensationManifoldAbsolutePressure();

	/// @return Fuel compensation in percentage.
	float IgnitionCompensationAirTemperature();

	/// @return Ignition compensation in degrees.
	float IgnitionCompensationCoolantTemperature();

	/// @return Ignition compensation in degrees.
	float IgnitionCompensationBarometer();

	/// @return Ignition compensation in degrees.
	float IgnitionCompensationManifoldAbsolutePressure();


private:
	void ProcessFramePe1();
	void ProcessFramePe2();
	void ProcessFramePe3();
	void ProcessFramePe4();
	void ProcessFramePe5();
	void ProcessFramePe6();
	void ProcessFramePe7();
	void ProcessFramePe8();
	void ProcessFramePe9();
	void ProcessFramePe10();
	void ProcessFramePe11();
	void ProcessFramePe12();
	void ProcessFramePe13();
	void ProcessFramePe14();
	void ProcessFramePe15();
	void ProcessFramePe16();

	const std::vector<uint32_t> can_id_list_ = {  FramePe1Id,
												  FramePe2Id,
												  FramePe3Id,
												  FramePe4Id,
												  FramePe5Id,
												  FramePe6Id,
												  FramePe7Id,
												  FramePe8Id,
												  FramePe9Id,
												  FramePe10Id,
												  FramePe11Id,
												  FramePe12Id,
												  FramePe13Id,
												  FramePe14Id,
												  FramePe15Id,
												  FramePe16Id  };

	std::shared_ptr<platform::ICan> can_bus_;
	uint8_t raw_data_[8];

	// Frame PE1 Fields
	uint16_t rpm_ = 0;
	float tps_ = 0.0f;
	float fuel_open_time_ = 0.0f;
	float ignition_angle_ = 0.0f;

	// Frame PE2 Fields
	float barometer_pressure_ = 0.0f;
	float map_ = 0.0f;
	float lambda_ = 0.0f;
	PressureType pressure_unit_ = PressureType::kUnknown;

	// Frame PE3 and PE4 Fields
	static constexpr uint8_t kNumAnalogPorts = 8;
	std::array<float, kNumAnalogPorts> analog_inputs_;

	//Frame PE5 Fields
	static constexpr uint8_t kNumFrequencyPorts = 4;
	std::array<float, kNumFrequencyPorts> frequencies_;

	// Frame PE6 Fields
	float battery_voltage_ = 0.0f;
	float air_temp_ = 0.0f;
	float coolant_temp_ = 0.0f;
	TemperatureType temperature_unit_ = TemperatureType::kUnknown;

	// Frame PE7 Fields
	static constexpr uint8_t kNumAnalogThermistorPorts = 2;
	std::array<float, kNumAnalogThermistorPorts> analog_inputs_thermistors_;

	// Frame PE8 Fields
	float rpm_rate_ = 0.0f;
	float tps_rate_ = 0.0f;
	float map_rate_ = 0.0f;
	float maf_load_rate_ = 0.0f;

	// Frame PE9 Fields
	static constexpr uint8_t kNumMeasuredLambda = 2;
	std::array<float, kNumMeasuredLambda> measured_lambdas_;
	float target_lambda_ = 0.0f;

	// Frame PE10 Fields
	static constexpr uint8_t kNumDutyCyclePorts = 8;
	std::array<float, kNumDutyCyclePorts> duty_cycles_;

	// Frame PE11 Fields
	float percent_slip_ = 0.0f;
	float driven_wheel_rate_ = 0.0f;
	float desired_value_ = 0.0f;

	// Frame PE12 Fields
	float driven_avg_wheel_speed_ = 0.0f;
	float nondriven_avg_wheel_speed_ = 0.0f;
	float ignition_compensation_ = 0.0f;
	float ignition_cut_percentage_ = 0.0f;

	// Frame PE13 Fields
	static constexpr uint8_t kNumWheelSpeeds = 2;
	std::array<float, kNumWheelSpeeds> driven_wheel_speeds_;
	std::array<float, kNumWheelSpeeds> nondriven_wheel_speeds_;

	// Frame PE14 Fields
	float fuel_comp_acceleration_ = 0.0f;
	float fuel_comp_starting_ = 0.0f;
	float fuel_comp_air_temp_ = 0.0f;
	float fuel_comp_coolant_temp_ = 0.0f;

	// Frame PE15 Fields
	float fuel_comp_barometer_ = 0.0f;
	float fuel_comp_map_ = 0.0f;

	// Frame PE16 Fields
	float ignition_comp_air_temp_ = 0.0f;
	float ignition_comp_coolant_temp_ = 0.0f;
	float ignition_comp_barometer_ = 0.0f;
	float ignition_comp_map_ = 0.0f;
};

} // namespace sensor

#endif // ECU_PE3_PE3_H
