/*
* DataAcquisition2.0 — 2023 Vehicle
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/


// Standard Libraries
#include <stdio.h>
#include <inttypes.h>

#include <memory>

// ST HAL Dependencies
#include "gpio.h"

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

#include "can.h"
extern CAN_HandleTypeDef hcan1;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

// DFR Custom Dependencies
#include "app.hpp"
#include "../../Core/Inc/retarget.h"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"
// variables
uint8_t TxData[8] = {0};
int counter = 0;
uint32_t TxMailbox;

// CallBack functions


//Can CallBack
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{

    uint8_t RxData[8];
    if (HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
    	  HAL_CAN_AddTxMessage(hcan1, &TxHeader,TxData,&TxMailbox);
    	  TxData[0] = 1 + counter;
    	  TxData[1] = 2 + counter;
    	  TxData[2] = 3 + counter;
    	  TxData[3] = 4 + counter;
    	  TxData[4] = 5 + counter;
    	  TxData[5] = 6 + counter;
    	  TxData[6] = 7 + counter;
    	  TxData[7] = 8 + counter;
    	  counter++;
    }
}



void cppMain() {
	// handling TX message
	TxHeader.ExtId = 0x102;
	TxHeader.DLC = 8;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.TransmitGlobalTime = DISABLE;
	// Enable `printf()` using USART
	RetargetInit(&huart3);

	std::unique_ptr<sensor::ILinearPotentiometer> lin_pot(nullptr);
	lin_pot = std::make_unique<sensor::SLS1322>(hadc1);

	float displacement_inches = 0.0f;

	for(;;) {
		// HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		displacement_inches = lin_pot->DisplacementInches();
		HAL_Delay(1000);
		printf("\n Percentage: %f", displacement_inches);
	}
}
