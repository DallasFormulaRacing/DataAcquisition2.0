/*
* Mutex CMSIS RTOS V2
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef MUTEX_CMSIS_RTOS_V2_H
#define MUTEX_CMSIS_RTOS_V2_H

// 3rd Party Libraries and Frameworks
#include "cmsis_os.h"

// DFR Custom Dependencies
#include "imutex.hpp"

namespace application {

class MutexCmsisV2 : public IMutex {
public:
	MutexCmsisV2(const osMutexAttr_t& attributes);

	virtual ~MutexCmsisV2();

	void Create();

	virtual void Lock() override;

	virtual void Unlock() override;

private:
	osMutexId_t mutex_id_;
	const osMutexAttr_t& attributes_;
};

} // namespace application

#endif // MUTEX_CMSIS_RTOS_V2_H
