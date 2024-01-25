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
#include <vector>


// ST HAL Dependencies
#include "gpio.h"

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

#include "can.h"
extern CAN_HandleTypeDef hcan1;
CAN_FilterTypeDef CAN;

#include "i2c.h"
extern I2C_HandleTypeDef hi2c1;

// DFR Custom Dependencies
#include "app.hpp"
#include "../../Core/Inc/retarget.h"
#include "Platform/CAN/STM/F4/bxcan_stmf4.hpp"
#include "Platform/CAN/Interfaces/ican.hpp"
#include "Sensor/Accelerometer/lsm303dlhc.hpp"
#include "Sensor/GyroScope/igyroscope.hpp"
#include "Sensor/GyroScope/l3gd20h.hpp"
#include "Sensor/ECU/PE3/Frames/frame_pe2.hpp"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"


// CAN Bus Interrupt Callback
std::shared_ptr<platform::BxCanStmF4> bx_can_callback_ptr(nullptr);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	bx_can_callback_ptr->ReceiveCallback();
}

using ReceiveInterruptMode = platform::BxCanStmF4::ReceiveInterruptMode;


void cppMain() {
	// Enable `printf()` using USART
	RetargetInit(&huart3);

//	std::unique_ptr<sensor::ILinearPotentiometer> lin_pot(nullptr);
//	lin_pot = std::make_unique<sensor::SLS1322>(hadc1);
//
//	std::unique_ptr<sensor::IAccelerometer> accelerometer(nullptr);
//	accelerometer = std::make_unique<sensor::LSM303DLHC>(hi2c1);
//	accelerometer->init();
//
//	std::unique_ptr<sensor::IGyroscope> gyroscope(nullptr);
//	gyroscope = std::make_unique<sensor::L3GD20H>(hi2c1);




	std::vector<uint32_t> can_id_list = { 0x0CFFF048,
										  0x0CFFF148,
										//   0x0CFFF248,
										//   0x0CFFF348,
										//   0x0CFFF448,
										  0x0CFFF548,
										//   0x0CFFF648,
										//   0x0CFFF748,
										//   0x0CFFF848,
										//   0x0CFFF948,
										//   0x0CFFFA48,
										//   0x0CFFFB48,
										//   0x0CFFFC48,
										//   0x0CFFFD48,
										//   0x0CFFFE48,
										  0x0CFFD048  };

	std::shared_ptr<platform::BxCanStmF4> bx_can_peripheral(nullptr);
	bx_can_peripheral = std::make_shared<platform::BxCanStmF4>(hcan1, CAN, can_id_list);

	ReceiveInterruptMode rx_interrupt_mode = ReceiveInterruptMode::kFifo0MessagePending;
	bx_can_peripheral->ConfigureReceiveCallback(rx_interrupt_mode);

	bx_can_callback_ptr = bx_can_peripheral;

	std::shared_ptr<platform::ICan> can_bus(nullptr);
	can_bus = bx_can_peripheral;
	// TODO: pass `can_bus` to ECU component via constructor
	can_bus->EnableInterruptMode();

	uint8_t rx_buffer[8];

//	float displacement_inches = 0.0f;
//	float* acc_data = 0.0f;
//	int16_t *gyro_data = 0;

	for(;;) {
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
//
//		displacement_inches = lin_pot->DisplacementInches();
//		printf("\n Percentage: %f", displacement_inches);
//
//		accelerometer->ComputeAcceleration();
//		acc_data = accelerometer->GetAcceleration();
//
//		printf("the x-axis is %lf \t\t " , acc_data[0]);
//		printf("the y-axis is %lf \t\t " , acc_data[1]);
//		printf("the z-axis is %lf " , acc_data[2]);
//		printf("\r");
//		printf("\n");
//
//		gyro_data = gyroscope->DegreesPerSecond();
//		printf("x = %hd\t",gyro_data[0]);
//		printf("y = %hd\t",gyro_data[1]);
//		printf("z = %hd\t",gyro_data[2]);
//		printf("\n");
//		printf("\r");



		//HAL_Delay(150);
		if(can_bus->MessageArrivedFlag()){
			__disable_irq();
//			can_bus->DisableInterruptMode();
			can_bus->Receive(rx_buffer);

			if(can_bus->LatestCanId() == 0x0CFFF148) {
				sensor::FramePE2 frame(rx_buffer);
				float value = frame.ManifoldAbsolutePressure();

				printf("\t\t 0x0CFFF148 \n");
				printf("\r");

			} else if(can_bus->LatestCanId() == 0x0CFFF548){
				printf("\t\t 0x0CFFF548 \n");
				printf("\r");
				// Process raw data
			}

			can_bus->ClearMessageArrivedFlag();
//			can_bus->EnableInterruptMode();
			__enable_irq();
		}
	}
}



