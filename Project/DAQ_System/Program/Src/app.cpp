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

#include "i2c.h"
extern I2C_HandleTypeDef hi2c1;

// 3rd Party Libraryes and Frameworks
#include "cmsis_os.h"
#include "fatfs.h"
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */
#include "usb_host.h"


// DFR Custom Dependencies
#include "app.hpp"
#include "Application/FileSystem/fat_fs.hpp"
#include "../../Core/Inc/retarget.h"
#include "Platform/CAN/STM/F4/bxcan_stmf4.hpp"
#include "Platform/CAN/Interfaces/ican.hpp"
#include "Sensor/Accelerometer/lsm303dlhc.hpp"
#include "Sensor/ECU/PE3/iecu.hpp"
#include "Sensor/ECU/PE3/pe3.hpp"
#include "Sensor/GyroScope/igyroscope.hpp"
#include "Sensor/GyroScope/l3gd20h.hpp"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"
#include "Sensor/Accelerometer/LSM6DSOXAccelerometer.hpp"

#include "Platform/GPIO/igpio.hpp"
#include "Platform/GPIO/gpio_stmf4.hpp"



std::shared_ptr<platform::GpioStmF4> gpio_callback_ptr(nullptr);
bool logging_mode_changed = false;
bool to_log = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	gpio_callback_ptr->InterruptCallback(GPIO_Pin);
}



#include "Application/data_payload.hpp"
#include "Application/DataLogger/DataLogger.hpp"





// CAN Bus Interrupt Callback
std::shared_ptr<platform::BxCanStmF4> bx_can_callback_ptr(nullptr);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	bx_can_callback_ptr->ReceiveCallback();
}

using ReceiveInterruptMode = platform::BxCanStmF4::ReceiveInterruptMode;

void RtosInit();
void DataLoggingThread(void *argument);

void cppMain() {
	// Enable `printf()` using USART
	RetargetInit(&huart3);


	auto bx_can_peripheral = std::make_shared<platform::BxCanStmF4>(hcan1);
	std::shared_ptr<platform::ICan> can_bus = bx_can_peripheral;

	auto pe3_ecu = std::make_unique<sensor::Pe3>(can_bus);
	const std::vector<uint32_t>& can_id_list = pe3_ecu->CanIdList();

	// Subscribe to messages with PE3's CAN IDs
	for (const uint32_t& can_id : can_id_list) {
		bx_can_peripheral->ConfigureFilter((can_id >> 13), (can_id & 0x1FFF));
	}

	bx_can_peripheral->Start();

	bx_can_callback_ptr = bx_can_peripheral;
	ReceiveInterruptMode rx_interrupt_mode = ReceiveInterruptMode::kFifo0MessagePending;
	bx_can_peripheral->ConfigureReceiveCallback(rx_interrupt_mode);
	bx_can_peripheral->EnableInterruptMode();

	float manifold_absolute_pressure = 0.0f;
	float battery_voltage = 0.0f;



//	NVIC_SetPriorityGrouping( 0 ); //TODO
//	osKernelInitialize();	// Initialize scheduler
//	RtosInit();				// Initialize thread
//	osKernelStart();		// Start scheduler

	auto switch_gpio_peripheral = std::make_shared<platform::GpioStmF4>(GPIOF, GPIO_PIN_15);
	std::shared_ptr<platform::IGpio> gpio = switch_gpio_peripheral;
	gpio_callback_ptr = switch_gpio_peripheral;


	for(;;) {
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		if (pe3_ecu->NewMessageArrived()) {
			__disable_irq();

			pe3_ecu->Update();
			uint32_t can_id = pe3_ecu->LatestCanId();

			switch(can_id) {
			case FramePe2Id:
				manifold_absolute_pressure = pe3_ecu->Map();

				printf("\t\t %" PRIu32 "\n", can_id);
				printf("Manifold Pressure: %f\n", manifold_absolute_pressure);
				printf("\r");
				break;


			case FramePe6Id:
				battery_voltage = pe3_ecu->BatteryVoltage();

				printf("\t\t %" PRIu32 "\n", can_id);
				printf("Battery Voltage: %f\n", battery_voltage);
				printf("\r");
				break;
			}

			__enable_irq();
		}



		if (gpio->ToggleDetected()) {
			if (gpio->Read() == true) {
				printf("high\n");
			} else {
				printf("low\n");
			}
		}


	}
}

osThreadId_t dataLoggingTaskHandle;
const osThreadAttr_t dataLoggingTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 17,
  .priority = (osPriority_t) osPriorityNormal,
};

void RtosInit() {
	dataLoggingTaskHandle = osThreadNew(DataLoggingThread, NULL, &dataLoggingTask_attributes);
}

void DataLoggingThread(void *argument) {
	MX_USB_HOST_Init();

	std::shared_ptr<application::IFileSystem> file_system(nullptr);
	file_system = std::make_shared<application::FatFs>(USBHPath, USBHFatFS, USBHFile);

	application::DataLogger data_logger(file_system);

	// Dummy data
	application::DataPayload dummy_data;
	dummy_data.timestamp_ = 15;
	dummy_data.linpot_displacement_inches_[0] = 2.5;
	dummy_data.linpot_displacement_inches_[1] = 0.5;
	dummy_data.linpot_displacement_inches_[2] = 1.3;
	dummy_data.linpot_displacement_inches_[3] = 4.0;


	bool ready_to_log = false;

	for (;;) {

		if(block_device_connected == 1) {
			// Standby state
			// TODO for transition: check GPIO, despite no interrupt.

			file_system->Mount();
			ready_to_log = true;
			HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);

			block_device_connected = 0;

		} else if (block_device_ejected == 1) {
			// Idle state

			// Edge case: if the usb flash drive is ejected while the switch is logical HIGH,
			//			  then the logging file is left open.
			if (to_log) {
				file_system->CloseFile();
				to_log = false;
			}

			file_system->Unmount();
			ready_to_log = false;

			block_device_ejected = 0;
		}


		if (ready_to_log) {

			if (logging_mode_changed) {
				if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_15) == GPIO_PIN_SET) {
					// Logging state

					to_log = true;
					// TODO: Create a new logging file here
				} else {
					to_log = false;
				}

				logging_mode_changed = false;
			}


			if (to_log) {
				printf("logging...\n");

//				char root_file_path[] = "/ROOTFILE.csv";
//				file_system->CreateFile(root_file_path);
//
//				char root_file_header_row[] = "LinPot1,LinPot2,AccX,AccY,AccZ\n";
//				char root_file_contents[] = "2.3,2.45,2,9,200\n";
//				file_system->OpenFile(root_file_path, (char*)"a");
//				file_system->WriteFile(root_file_header_row);
//				file_system->WriteFile(root_file_contents);
//				file_system->WriteFile(root_file_contents);


			} else {
				printf("stop\n");

//				file_system->CloseFile();
			}

		}

		HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET);
	}
}


