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

	// TODO: getter methods for each variable

};

}

#endif // IECU_H
