/*
* ECU Abstract Interface
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IECU_H
#define IECU_H

#include <vector>

namespace sensor {

class IEcu {
public:
	virtual ~IEcu() = default;

	virtual const std::vector<uint32_t>& CanIdList() = 0;

	virtual uint32_t LatestCanId() = 0;

	virtual bool NewMessageArrived() = 0;

	virtual void Update() = 0;

	virtual int16_t Rpm() = 0;

	virtual float Tps() = 0;

	virtual float FuelOpenTime() = 0;

	virtual float IgnitionAngle() = 0;

	virtual float BarometerPressure() = 0;

	virtual float Map() = 0;

	virtual float Lambda() = 0;

	virtual float BatteryVoltage() = 0;

	virtual float AirTemperature() = 0;

	virtual float CoolantTemperature() = 0;

	virtual float AnalogInputVoltage(uint8_t index) = 0;

};

}

#endif // IECU_H
