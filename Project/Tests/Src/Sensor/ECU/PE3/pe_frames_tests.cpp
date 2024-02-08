/*
* ECU PE3 Frames Unit Tests
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
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe1.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe2.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe3.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe4.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe5.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe6.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe7.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe8.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe9.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe10.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe11.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe12.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe13.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe14.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe15.hpp"
#include "../../../../../../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe16.hpp"

namespace sensor {

TEST(Pe3EcuFrames, FramePe1) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x1E, 0x05,
                                                0xA5, 0x03,
                                                0xFE, 0x00,
                                                0x3C, 0x00  };
    FramePE1 frame(rx_buffer);

    static constexpr uint16_t kExpectedRpm = 1310;
    static constexpr float kExpectedTps = 93.3f;
    static constexpr float kExpectedFuelOpenTime = 25.4f;
    static constexpr float kExpectedIgnitionAngle = 6.0f;

    EXPECT_EQ(frame.RevolutionsPerMinute(), kExpectedRpm);
    EXPECT_FLOAT_EQ(frame.ThrottlePosition(), kExpectedTps);
    EXPECT_FLOAT_EQ(frame.FuelOpenTime(), kExpectedFuelOpenTime);
    EXPECT_FLOAT_EQ(frame.IgnitionAngle(), kExpectedIgnitionAngle);
}

TEST(Pe3EcuFrames, FramePe2PressureKiloPascal) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x05,
                                                0xBB, 0x05,
                                                0x8F, 0x03,
                                                0x01, 0xFF  };
    FramePE2 frame(rx_buffer);

    static constexpr float kExpectedBarometerPressure = 14.7f;
    static constexpr float kExpectedMap = 14.67f;
    static constexpr float kExpectedLambda = 9.11f;
    static constexpr PressureType kExpectedPressureUnit = PressureType::kKiloPascal;

    EXPECT_FLOAT_EQ(frame.BarometerPressure(), kExpectedBarometerPressure);
    EXPECT_FLOAT_EQ(frame.ManifoldAbsolutePressure(), kExpectedMap);
    EXPECT_FLOAT_EQ(frame.Lambda(), kExpectedLambda);
    EXPECT_EQ(frame.PressureUnit(), kExpectedPressureUnit);
}

TEST(Pe3EcuFrames, FramePe2PressurePoundsPerSquareInch) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x05,
                                                0xBB, 0x05,
                                                0x8F, 0x03,
                                                0x00, 0xFF  };
    FramePE2 frame(rx_buffer);

    static constexpr PressureType kExpectedPressureUnit = PressureType::kPoundsPerSquareInch;
    EXPECT_EQ(frame.PressureUnit(), kExpectedPressureUnit);
}

TEST(Pe3EcuFrames, FramePe2PressureUnkown) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x05,
                                                0xBB, 0x05,
                                                0x8F, 0x03,
                                                0x0A, 0xFF  };
    FramePE2 frame(rx_buffer);

    static constexpr PressureType kExpectedPressureUnit = PressureType::kUnknown;
    EXPECT_EQ(frame.PressureUnit(), kExpectedPressureUnit);
}

TEST(Pe3EcuFrames, FramePe3) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x33, 0x04,
                                                0x01, 0x08,
                                                0x06, 0x00,
                                                0x0A, 0x02  };
    FramePE3 frame(rx_buffer);

    static constexpr float kExpectedVoltages[frame.kNumOfFields] = {  1.075f,
                                                                      2.049f,
                                                                      0.006f,
                                                                      0.522f  };
    for (int i = 0; i < frame.kNumOfFields; i++) {
        EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(i), kExpectedVoltages[i]);
    }
}

TEST(Pe3EcuFrames, FramePe3OutOfBounds) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x33, 0x04,
                                                0x01, 0x08,
                                                0x06, 0x00,
                                                0x0A, 0x02  };
    FramePE3 frame(rx_buffer);

    static constexpr float kExpecteResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(kMaxIndex + 1), kExpecteResult);
    EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(kMaxIndex + 2), kExpecteResult);
}

TEST(Pe3EcuFrames, FramePe4) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x04, 0x00,
                                                0x09, 0x02,
                                                0x05, 0x00,
                                                0x25, 0x00  };
    FramePE4 frame(rx_buffer);

    static constexpr float kExpectedVoltages[frame.kNumOfFields] = {  0.004f,
                                                                      0.521f,
                                                                      0.005f,
                                                                      0.037f  };
    for (int i = 0; i < frame.kNumOfFields; i++) {
        EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(i), kExpectedVoltages[i]);
    }
}

TEST(Pe3EcuFrames, FramePe4OutOfBounds) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x04, 0x00,
                                                0x09, 0x02,
                                                0x05, 0x00,
                                                0x25, 0x00  };
    FramePE4 frame(rx_buffer);

    static constexpr float kExpecteResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(kMaxIndex + 1), kExpecteResult);
    EXPECT_FLOAT_EQ(frame.AnalogInputVoltage(kMaxIndex + 2), kExpecteResult);
}

TEST(Pe3EcuFrames, FramePe5) {
    // Dummy data. TODO: Gather and analyze sample data when the Digital Inputs
    // are configured to measure frequency (page 60 of the PE3 Series manual).
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE5 frame(rx_buffer);

    static constexpr float kExpectedFrequencies[frame.kNumOfFields] = {  0.2f,
                                                                         0.4f,
                                                                         0.6f,
                                                                         0.8f  };
    for (int i = 0; i < frame.kNumOfFields; i++) {
        EXPECT_FLOAT_EQ(frame.FrequencyHertz(i), kExpectedFrequencies[i]);
    }
}

TEST(Pe3EcuFrames, FramePe5OutOfBounds) {
    // Dummy data. TODO: Gather and analyze sample data when the Digital Inputs
    // are configured to measure frequency (page 60 of the PE3 Series manual).
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE5 frame(rx_buffer);

    static constexpr float kExpecteResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.FrequencyHertz(kMaxIndex + 1), kExpecteResult);
    EXPECT_FLOAT_EQ(frame.FrequencyHertz(kMaxIndex + 2), kExpecteResult);
}

TEST(Pe3EcuFrames, FramePe6TempCelsius) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x14, 0x05,
                                                0xAA, 0x02,
                                                0xCE, 0x02,
                                                0x01, 0xFF  };
    FramePE6 frame(rx_buffer);

    static constexpr float kExpectedBatteryVoltage = 13.0f;
    static constexpr float kExpectedAirTemp = 68.2f;
    static constexpr float kExpectedCoolantTemp = 71.8f;
    static constexpr TemperatureType kExpectedTemperatureUnit = TemperatureType::kCelsius;

    EXPECT_FLOAT_EQ(frame.BatteryVoltage(), kExpectedBatteryVoltage);
    EXPECT_FLOAT_EQ(frame.AirTemperature(), kExpectedAirTemp);
    EXPECT_FLOAT_EQ(frame.CoolantTemperature(), kExpectedCoolantTemp);
    EXPECT_EQ(frame.TemperatureUnit(), kExpectedTemperatureUnit);
}

TEST(Pe3EcuFrames, FramePe6TempFaranheit) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x14, 0x05,
                                                0xAA, 0x02,
                                                0xCE, 0x02,
                                                0x00, 0x00  };
    FramePE6 frame(rx_buffer);

    static constexpr TemperatureType kExpectedTemperatureUnit = TemperatureType::kFarenheit;
    EXPECT_EQ(frame.TemperatureUnit(), kExpectedTemperatureUnit);
}

TEST(Pe3EcuFrames, FramePe6TempUnknown) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x14, 0x05,
                                                0xAA, 0x02,
                                                0xCE, 0x02,
                                                0xCC, 0xFF  };
    FramePE6 frame(rx_buffer);

    static constexpr TemperatureType kExpectedTemperatureUnit = TemperatureType::kUnknown;
    EXPECT_EQ(frame.TemperatureUnit(), kExpectedTemperatureUnit);
}

TEST(Pe3EcuFrames, FramePe7) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xA8, 0x02,
                                                0xA8, 0x02,
                                                0x03, 0x04,
                                                0x3C, 0xFF  };
    FramePE7 frame(rx_buffer);

    static constexpr float kExpectedVoltage = 68.0f;
    EXPECT_FLOAT_EQ(frame.AnalogInputThermistorVoltage(0), kExpectedVoltage);
    EXPECT_FLOAT_EQ(frame.AnalogInputThermistorVoltage(1), kExpectedVoltage);
}

TEST(Pe3EcuFrames, FramePe7OutOfBounds) {
    // Sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xA8, 0x02,
                                                0xA8, 0x02,
                                                0x03, 0x04,
                                                0x3C, 0xFF  };
    FramePE7 frame(rx_buffer);

    static constexpr float kExpecteResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.AnalogInputThermistorVoltage(kMaxIndex + 1), kExpecteResult);
    EXPECT_FLOAT_EQ(frame.AnalogInputThermistorVoltage(kMaxIndex + 2), kExpecteResult);
}

TEST(Pe3EcuFrames, FramePe8) {
    // Dummy data. TODO: Gather and analyze sample data while vehicle is in motion.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE8 frame(rx_buffer);

    static constexpr float kExpectedRpmRate = 1.0f;
    static constexpr float kExpectedTpsRate = 2.0f;
    static constexpr float kExpectedMapRate = 3.0f;
    static constexpr float kExpectedMafLoadRate = 0.4f;

    EXPECT_FLOAT_EQ(frame.RevolutionsPerMinuteRate(), kExpectedRpmRate);
    EXPECT_FLOAT_EQ(frame.ThrottlePositionPercentageRate(), kExpectedTpsRate);
    EXPECT_FLOAT_EQ(frame.ManifoldAbsolutePressureRate(), kExpectedMapRate);
    EXPECT_FLOAT_EQ(frame.MassAirFlowLoadRate(), kExpectedMafLoadRate);
}

TEST(Pe3EcuFrames, FramePe9) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x03,
                                                0x00, 0x00,
                                                0xB6, 0x03,
                                                0x00, 0x00  };
    FramePE9 frame(rx_buffer);

    static constexpr float kExpectedMeasuredLambda1 = 9.58f;
    static constexpr float kExpectedMeasuredLambda2 = 0.0f;
    static constexpr float kExpectedTargetLambda = 9.5f;

    EXPECT_FLOAT_EQ(frame.LambdaMeasured(0), kExpectedMeasuredLambda1);
    EXPECT_FLOAT_EQ(frame.LambdaMeasured(1), kExpectedMeasuredLambda2);
    EXPECT_FLOAT_EQ(frame.TargetLambda(), kExpectedTargetLambda);
}

TEST(Pe3EcuFrames, FramePe9OutOfBounds) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xBE, 0x03,
                                                0x00, 0x00,
                                                0xB6, 0x03,
                                                0x00, 0x00  };
    FramePE9 frame(rx_buffer);

    static constexpr float kExpectedResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.LambdaMeasured(kMaxIndex + 1), kExpectedResult);
    EXPECT_FLOAT_EQ(frame.LambdaMeasured(kMaxIndex + 2), kExpectedResult);
}

TEST(Pe3EcuFrames, FramePe10) {
    // Dummy data. TODO: Gather and analyze sample data with vehicle
    // in motion while PWM outuput duty cycle is enabled.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE10 frame(rx_buffer);
    
    static constexpr float kExpectedDutyCycles[frame.kNumOfFields] = {  0.5f, 0.0f,
                                                                        1.0f, 0.0f,
                                                                        1.5f, 0.0f,
                                                                        2.0f, 0.0f  };
    for (int i = 0; i < frame.kNumOfFields; i++) {
        EXPECT_FLOAT_EQ(frame.PwmDutyCycle(i), kExpectedDutyCycles[i]);
    }
}

TEST(Pe3EcuFrames, FramePe10OutputIsOff) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x00, 0x00,
                                                0x00, 0x00,
                                                0x00, 0x00,
                                                0x00, 0x00  };
    FramePE10 frame(rx_buffer);
    
    static constexpr float kExpectedDutyCycle = 0.0f;
    for (int i = 0; i < frame.kNumOfFields; i++) {
        EXPECT_FLOAT_EQ(frame.PwmDutyCycle(i), kExpectedDutyCycle);
    }
}

TEST(Pe3EcuFrames, FramePe10OutOfBounds) {
    // Dummy data. TODO: Gather and analyze sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE10 frame(rx_buffer);
    
    static constexpr float kExpectedResult = 0.0f;
    static constexpr uint8_t kMaxIndex = frame.kNumOfFields;

    EXPECT_FLOAT_EQ(frame.PwmDutyCycle(kMaxIndex + 1), kExpectedResult);
    EXPECT_FLOAT_EQ(frame.PwmDutyCycle(kMaxIndex + 2), kExpectedResult);
}

TEST(Pe3EcuFrames, FramePe11) {
    // Sample data.
    // TODO: Gather sample data while the vehicle is in motion for 
    //       the Percent Slip and Driven Wheel Rate of Change fields.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x00, 0x00,
                                                0x00, 0x00,
                                                0x96, 0x00,
                                                0xFF, 0xFF  };
    FramePE11 frame(rx_buffer);

    static constexpr float kExpectedPercentSlip = 0.0f;
    static constexpr float kExpectedDrivenWheelRate = 0.0f;
    static constexpr float kExpectedDesiredValue = 15.0f;

    EXPECT_FLOAT_EQ(frame.PercentSlip(), kExpectedPercentSlip);
    EXPECT_FLOAT_EQ(frame.DrivenWheelRateOfChange(), kExpectedDrivenWheelRate);
    EXPECT_FLOAT_EQ(frame.DesiredValue(), kExpectedDesiredValue);
}

TEST(Pe3EcuFrames, FramePe12) {
    // Dummy data. TODO: Gather and analyze sample data while the vehicle is in motion.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE12 frame(rx_buffer);

    static constexpr float kExpectedDrivenAverageWheelSpeed = 0.1f;
    static constexpr float kExpectedNonDrivenAverageWheelSpeed = 0.2f;
    static constexpr float kExpectedIgnitionCompensation = 0.3f;
    static constexpr float kExpectedIgnitionCutPercentage = 0.4f;

    EXPECT_FLOAT_EQ(frame.DrivenAverageWheelSpeed(), kExpectedDrivenAverageWheelSpeed);
    EXPECT_FLOAT_EQ(frame.NonDrivenAverageWheelSpeed(), kExpectedNonDrivenAverageWheelSpeed);
    EXPECT_FLOAT_EQ(frame.IgnitionCompensation(), kExpectedIgnitionCompensation);
    EXPECT_FLOAT_EQ(frame.IgnitionCutPercentage(), kExpectedIgnitionCutPercentage);
}

TEST(Pe3EcuFrames, FramePe13) {
    // Dummy data. TODO: Gather and analyze sample data while the vehicle is in motion.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE13 frame(rx_buffer);

    static constexpr float kExpectedDrivenWheelSpeed1 = 0.1f;
    static constexpr float kExpectedDrivenWheelSpeed2 = 0.2f;
    static constexpr float kExpectedNonDrivenWheelSpeed1 = 0.3f;
    static constexpr float kExpectedNonDrivenWheelSpeed2 = 0.4f;

    EXPECT_FLOAT_EQ(frame.DrivenWheelSpeed(0), kExpectedDrivenWheelSpeed1);
    EXPECT_FLOAT_EQ(frame.DrivenWheelSpeed(1), kExpectedDrivenWheelSpeed2);
    EXPECT_FLOAT_EQ(frame.NonDrivenWheelSpeed(0), kExpectedNonDrivenWheelSpeed1);
    EXPECT_FLOAT_EQ(frame.NonDrivenWheelSpeed(1), kExpectedNonDrivenWheelSpeed2);
}

TEST(Pe3EcuFrames, FramePe13OutOfBounds) {
    // Dummy data. TODO: Gather and analyze sample data
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x01, 0x00,
                                                0x02, 0x00,
                                                0x03, 0x00,
                                                0x04, 0x00  };
    FramePE13 frame(rx_buffer);

    static constexpr float kExpectedResult = 0.0f;
    EXPECT_FLOAT_EQ(frame.DrivenWheelSpeed(2), kExpectedResult);
    EXPECT_FLOAT_EQ(frame.DrivenWheelSpeed(3), kExpectedResult);
    EXPECT_FLOAT_EQ(frame.NonDrivenWheelSpeed(2), kExpectedResult);
    EXPECT_FLOAT_EQ(frame.NonDrivenWheelSpeed(3), kExpectedResult);
}

TEST(Pe3EcuFrames, FramePe14) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xE8, 0x03,
                                                0xD9, 0x04,
                                                0xFF, 0x03,
                                                0xE8, 0x03  };
    FramePE14 frame(rx_buffer);

    static constexpr float kExpectedFuelCompAcceleration = 100.0f;
    static constexpr float kExpectedFuelCompStarting = 124.1f;
    static constexpr float kExpectedFuelCompAirTemp = 102.3f;
    static constexpr float kExpectedFuelCompCoolantTemp = 100.0f;

    EXPECT_FLOAT_EQ(frame.FuelCompensationAcceleration(), kExpectedFuelCompAcceleration);
    EXPECT_FLOAT_EQ(frame.FuelCompensationStarting(), kExpectedFuelCompStarting);
    EXPECT_FLOAT_EQ(frame.FuelCompensationAirTemperature(), kExpectedFuelCompAirTemp);
    EXPECT_FLOAT_EQ(frame.FuelCompensationCoolantTemperature(), kExpectedFuelCompCoolantTemp);
}

TEST(Pe3EcuFrames, FramePe15) {
    // Sample data.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0xE8, 0x03,
                                                0xE8, 0x03,
                                                0xFF, 0xFF,
                                                0xFF, 0xFF  };
    FramePE15 frame(rx_buffer);

    static constexpr float kExpectedFuelCompBarometer = 100.0f;
    static constexpr float kExpectedFuelCompMap = 100.0f;

    EXPECT_FLOAT_EQ(frame.FuelCompensationBarometer(), kExpectedFuelCompBarometer);
    EXPECT_FLOAT_EQ(frame.FuelCompensationManifoldAbsolutePressure(), kExpectedFuelCompMap);
}

TEST(Pe3EcuFrames, FramePe16) {
    // Sample data.
    // TODO: Gather more sample data while vehicle is in motion.
    uint8_t rx_buffer[kByteArrayMaxLength] = {  0x00, 0x00,
                                                0x0A, 0x00,
                                                0x00, 0x00,
                                                0x00, 0x00  };
    FramePE16 frame(rx_buffer);

    static constexpr float kExpectedIgnitionCompAirTemp = 0.0f;
    static constexpr float kExpectedIgnitionCompCoolantTemp = 1.0f;
    static constexpr float kExpectedIgnitionCompBarometer = 0.0f;
    static constexpr float kExpectedIgnitionCompMap = 0.0f;

    EXPECT_FLOAT_EQ(frame.IgnitionCompensationAirTemperature(), kExpectedIgnitionCompAirTemp);
    EXPECT_FLOAT_EQ(frame.IgnitionCompensationCoolantTemperature(), kExpectedIgnitionCompCoolantTemp);
    EXPECT_FLOAT_EQ(frame.IgnitionCompensationBarometer(), kExpectedIgnitionCompBarometer);
    EXPECT_FLOAT_EQ(frame.IgnitionCompensationManifoldAbsolutePressure(), kExpectedIgnitionCompMap);
}

}