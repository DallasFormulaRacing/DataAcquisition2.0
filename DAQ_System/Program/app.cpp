/*
* DataAcquisition2.0 — 2023 Vehicle
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/


// Standard Libraries
#include <stdio.h>
#include <inttypes.h>

// ST HAL Dependencies
#include "gpio.h"

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

// DFR Custom Dependencies
#include "app.hpp"
#include "../Core/Inc/retarget.h"
#include "Src/Sensor/LinearPotentiometer/sls1322.hpp"


void cppMain() {
	// Enable `printf()` using USART
	RetargetInit(&huart3);

	sensor::SLS1322 lin_pot(hadc1);
	float displacement_inches = 0.0f;

	for(;;) {
		// HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		displacement_inches = lin_pot.DisplacementInches();
		HAL_Delay(1000);
//		printf("\n Percentage: %f", displacement_inches);
	}
}
