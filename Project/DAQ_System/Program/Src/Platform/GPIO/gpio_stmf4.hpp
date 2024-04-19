/*
* STM32 F4 GPIO Peripheral
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef STM_F4_GPIO_STMF4_H
#define STM_F4_GPIO_STMF4_H

// ST HAL Dependencies
#include "gpio.h"

// DFR Custom Dependencies
#include "igpio.hpp"


namespace platform {

class GpioStmF4 : public IGpio {
public:
	GpioStmF4(GPIO_TypeDef* port, uint16_t pin);

	virtual ~GpioStmF4();

	virtual bool Read() override;

	virtual void Write(bool signal) override;

	virtual bool ToggleDetected() override;

	void InterruptCallback(uint16_t pin_detected);

private:
	GPIO_TypeDef* port_;
	uint16_t pin_;
	bool interrupt_flag_ = false;

};

} // namespace platform

#endif // STM_F4_GPIO_STMF4_H
