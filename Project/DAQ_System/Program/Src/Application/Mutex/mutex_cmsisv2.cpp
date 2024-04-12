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

#include "mutex_cmsisv2.hpp"


namespace application {

MutexCmsisV2::MutexCmsisV2(const osMutexAttr_t& attributes)
  : attributes_(attributes) { }

MutexCmsisV2::~MutexCmsisV2() { }

void MutexCmsisV2::Create() {
	mutex_id_ = osMutexNew(&attributes_);
}

void MutexCmsisV2::Lock() {
	osMutexAcquire(mutex_id_, osWaitForever);
}

void MutexCmsisV2::Unlock() {
	osMutexRelease(mutex_id_);
}



} // namespace application
