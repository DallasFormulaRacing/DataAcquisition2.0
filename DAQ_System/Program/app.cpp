#include "app.hpp"

#include <stdio.h>
#include <inttypes.h>

#include "../Core/Inc/retarget.h"

#include "gpio.h"

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

void cppMain() {

	RetargetInit(&huart3);
	uint32_t value_adc = 0;

	// Main Loop
	for(;;) {
		// HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		printf("\r\nADC Value: ");
		printf("%" PRIu32 "\n", value_adc);

		HAL_Delay(100);
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		value_adc = HAL_ADC_GetValue(&hadc1);

	}
}
