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

#include "pe3.hpp"

namespace sensor {

Pe3::Pe3(std::shared_ptr<platform::ICan> can_bus)
  : can_bus_(can_bus) {

	can_bus_->SubscribeCanId(can_id_list_);
	can_bus_->Start();
}

Pe3::~Pe3() { }

const std::vector<uint32_t>& Pe3::CanIdList() {
	return can_id_list_;
}

uint32_t Pe3::LatestCanId() {
	return can_bus_->LatestCanId();
}

bool Pe3::NewMessageArrived() {
	return can_bus_->MessageArrivedFlag();
}

void Pe3::Update() {
	can_bus_->Receive(raw_data_);

	switch(can_bus_->LatestCanId()) {
	case FramePe1Id:
		ProcessFramePe1();
		break;
	case FramePe2Id:
		ProcessFramePe2();
		break;
	case FramePe3Id:
		ProcessFramePe3();
		break;
	case FramePe4Id:
		ProcessFramePe4();
		break;
	case FramePe5Id:
		ProcessFramePe5();
		break;
	case FramePe6Id:
		ProcessFramePe6();
		break;
	case FramePe7Id:
		ProcessFramePe7();
		break;
	case FramePe8Id:
		ProcessFramePe8();
		break;
	case FramePe9Id:
		ProcessFramePe9();
		break;
	case FramePe10Id:
		ProcessFramePe10();
		break;
	case FramePe11Id:
		ProcessFramePe11();
		break;
	case FramePe12Id:
		ProcessFramePe12();
		break;
	case FramePe13Id:
		ProcessFramePe13();
		break;
	case FramePe14Id:
		ProcessFramePe14();
		break;
	case FramePe15Id:
		ProcessFramePe15();
		break;
	case FramePe16Id:
		ProcessFramePe16();
		break;
	}

	can_bus_->ClearMessageArrivedFlag();
}

void Pe3::ProcessFramePe1() {
	FramePE1 frame(raw_data_);
	rpm_ = frame.RevolutionsPerMinute();
	tps_ = frame.ThrottlePosition();
	fuel_open_time_ = frame.FuelOpenTime();
	ignition_angle_ = frame.IgnitionAngle();
}

void Pe3::ProcessFramePe2() {
	FramePE2 frame(raw_data_);
	barometer_pressure_ = frame.BarometerPressure();
	map_ = frame.ManifoldAbsolutePressure();
	lambda_ = frame.Lambda();
	pressure_unit = frame.PressureUnit();
}

void Pe3::ProcessFramePe3() {
	FramePE3 frame(raw_data_);
	for (int i = 0; i < frame.kNumOfFields; i++) {
		analog_inputs_[i] = frame.AnalogInputVoltage(i);
	}
}

void Pe3::ProcessFramePe4() {
	FramePE4 frame(raw_data_);
	static constexpr uint8_t offset = 4;
	for (int i = 0; i < frame.kNumOfFields; i++) {
		analog_inputs_[i + offset] = frame.AnalogInputVoltage(i);
	}
}

void Pe3::ProcessFramePe5() {
	FramePE5 frame(raw_data_);
	for (int i = 0; i < frame.kNumOfFields; i++) {
		frequencies_[i] = frame.FrequencyHertz(i);
	}
}

void Pe3::ProcessFramePe6() {
	FramePE6 frame(raw_data_);
	battery_voltage_ = frame.BatteryVoltage();
	air_temp_ = frame.AirTemperature();
	coolant_temp_ = frame.CoolantTemperature();
	temperature_unit_ = frame.TemperatureUnit();
}

void Pe3::ProcessFramePe7() {
	FramePE7 frame(raw_data_);
	analog_inputs_thermistors_[0] = frame.AnalogInputVoltage(0);
	analog_inputs_thermistors_[1] = frame.AnalogInputVoltage(1);
}

void Pe3::ProcessFramePe8() {
	FramePE8 frame(raw_data_);
	rpm_rate_ = frame.RevolutionsPerMinuteRate();
	tps_rate_ = frame.ThrottlePositionPercentageRate();
	map_rate_ = frame.ManifoldAbsolutePressureRate();
	maf_load_rate_ = frame.MassAirFlowLoadRate();
}

void Pe3::ProcessFramePe9() {
	FramePE9 frame(raw_data_);
	measured_lambda_1_ = frame.LambdaMeasured(0);
	measured_lambda_2_ = frame.LambdaMeasured(1);
	target_lambda_ = frame.TargetLambda();
}

void Pe3::ProcessFramePe10() {
	FramePE10 frame(raw_data_);
	for (int i = 0; i < frame.kNumOfFields; i++) {
		duty_cycles_[i] = frame.PwmDutyCycle(i);
	}
}

void Pe3::ProcessFramePe11() {
	FramePE11 frame(raw_data_);
	percent_slip_ = frame.PercentSlip();
	driven_wheel_rate_ = frame.DrivenWheelRateOfChange();
	desired_value_ = frame.DesiredValue();
}

void Pe3::ProcessFramePe12() {
	FramePE12 frame(raw_data_);
	driven_avg_wheel_speed_ = frame.DrivenAverageWheelSpeed();
	nondriven_avg_wheel_speed_ = frame.NonDrivenAverageWheelSpeed();
	ignition_compensation_ = frame.IgnitionCompensation();
	ignition_cut_percentage_ = frame.IgnitionCutPercentage();
}

void Pe3::ProcessFramePe13() {
	FramePE13 frame(raw_data_);
	driven_wheel_speed_1_ = frame.DrivenWheelSpeed(0);
	driven_wheel_speed_2_ = frame.DrivenWheelSpeed(1);
	nondriven_wheel_speed_1_ = frame.NonDrivenWheelSpeed(0);
	nondriven_wheel_speed_2_ = frame.NonDrivenWheelSpeed(1);
}

void Pe3::ProcessFramePe14() {
	FramePE14 frame(raw_data_);
	fuel_comp_acceleration_ = frame.FuelCompensationAcceleration();
	fuel_comp_starting_ = frame.FuelCompensationStarting();
	fuel_comp_air_temp_ = frame.FuelCompensationAirTemperature();
	fuel_comp_coolant_temp_ = frame.FuelCompensationCoolantTemperature();
}

void Pe3::ProcessFramePe15() {
	FramePE15 frame(raw_data_);
	fuel_comp_barometer_ = frame.FuelCompensationBarometer();
	fuel_comp_map_ = frame.FuelCompensationManifoldAbsolutePressure();
}

void Pe3::ProcessFramePe16() {
	FramePE16 frame(raw_data_);
	ignition_comp_air_temp_ = frame.IgnitionCompensationAirTemperature();
	ignition_comp_coolant_temp_ = frame.IgnitionCompensationCoolantTemperature();
	ignition_comp_barometer_ = frame.IgnitionCompensationBarometer();
	ignition_comp_map_ = frame.IgnitionCompensationManifoldAbsolutePressure();
}


}

