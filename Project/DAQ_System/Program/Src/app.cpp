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
CAN_FilterTypeDef CAN;
CAN_FilterTypeDef CAN_two;
uint32_t canID;


// DFR Custom Dependencies
#include "app.hpp"
#include "../../Core/Inc/retarget.h"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"

// CAN variables
uint8_t TxData[8] = {0};
uint8_t RxData[8];
uint8_t HexToDecData[8] = {0};
uint32_t TxMailbox;
bool CANMessageReceivedFlag = false;

// CallBack functions


//Can CallBack
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{


    if (HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
    	CANMessageReceivedFlag = true;
    	canID = RxHeader.ExtId;


    }
}

void cppMain() {


	// Filter initialization
	CAN.FilterIdHigh = 0x0CFFF048 >> 13 ;
	CAN.FilterIdLow = ((0x0CFFF048 & 0x1FFF) <<3) | CAN_ID_EXT | CAN_RTR_DATA ;
	CAN.FilterMaskIdHigh = 0xFFFF;
	CAN.FilterMaskIdLow = 0xFFFF;
	CAN.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN.FilterBank = 10;
	CAN.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN.FilterActivation = CAN_FILTER_ENABLE;
	CAN.SlaveStartFilterBank = 13;
	HAL_CAN_ConfigFilter(&hcan1,&CAN);
	// Filter initialization
	CAN_two.FilterIdHigh = 0x0CFFF548 >> 13 ;
	CAN_two.FilterIdLow = ((0x0CFFF548 & 0x1FFF) <<3) | CAN_ID_EXT | CAN_RTR_DATA ;
	CAN_two.FilterMaskIdHigh = 0xFFFF;
	CAN_two.FilterMaskIdLow = 0xFFFF;
	CAN_two.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_two.FilterBank = 9;
	CAN_two.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_two.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_two.FilterActivation = CAN_FILTER_ENABLE;
	CAN_two.SlaveStartFilterBank = 13;
	HAL_CAN_ConfigFilter(&hcan1,&CAN_two);

	// handling TX message
	TxHeader.ExtId = 0x123;
	TxHeader.DLC = 8;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.TransmitGlobalTime = DISABLE;

	HAL_CAN_Start(&hcan1); // starts the CAN bus
	HAL_CAN_ActivateNotification( &hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // activate RX message interrupts


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


		printf("\n Percentage: %f", displacement_inches);
		if(CANMessageReceivedFlag){
			__disable_irq();
			TxData[0] = 1 ;
			TxData[1] = 2 ;
			TxData[2] = 3 ;
			TxData[3] = RxData[3] ;
			TxData[4] = RxData[4] ;
			TxData[5] = 6 ;
			TxData[6] = 0x12 ;
			TxData[7] = (RxData[4] * 256 + RxData[3]) * .1;

			HAL_CAN_AddTxMessage(&hcan1, &TxHeader,TxData,&TxMailbox);
			CANMessageReceivedFlag = false;
			__enable_irq();

		}
	}
}
