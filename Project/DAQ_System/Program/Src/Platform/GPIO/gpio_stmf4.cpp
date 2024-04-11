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

#include "gpio_stmf4.hpp"

namespace platform {

GpioStmF4::GpioStmF4(GPIO_TypeDef* port, uint16_t pin)
  : port_(port), pin_(pin) { }

GpioStmF4::~GpioStmF4() { }

bool GpioStmF4::Read() {
	return HAL_GPIO_ReadPin(port_, pin_);
}

void GpioStmF4::Write(bool signal) {
	GPIO_PinState pin_state;

	if (signal) {
		pin_state = GPIO_PIN_SET;
	} else {
		pin_state = GPIO_PIN_RESET;
	}

	HAL_GPIO_WritePin(port_, pin_, pin_state);
}

bool GpioStmF4::ToggleDetected() {
	bool is_toggle_detected = interrupt_flag_;
	interrupt_flag_ = false;
	return is_toggle_detected;
}

void GpioStmF4::InterruptCallback(uint16_t pin_detected) {
	if(pin_detected == pin_) {
		interrupt_flag_ = true;
	}
}

} // namespace platform
