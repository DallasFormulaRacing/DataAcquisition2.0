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

#include "tim.h"
extern TIM_HandleTypeDef htim7;

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

osTimerId_t one_shot_id, periodic_id;

static void one_shot_Callback (void *argument) {
  int32_t arg = (int32_t)argument; // cast back argument '0'
  // do something, i.e. set thread/event flags
  uint32_t timeInMilliseconds = (osKernelGetTickCount()/osKernelGetTickFreq()) * 1000;
  printf("one shot timer: %d\n", timeInMilliseconds);

}
static void periodic_Callback (void *argument) {
  int32_t arg = (int32_t)argument; // cast back argument '5'
  // do something, i.e. set thread/event flags
  uint64_t timeInMilliseconds = (osKernelGetTickCount()/osKernelGetTickFreq()) * 1000;
  printf("periodic timer: %llu\n", static_cast<unsigned long long>(timeInMilliseconds));
  //osDelay(2000);
}

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

//	std::unique_ptr<sensor::ILinearPotentiometer> lin_pot(nullptr);
//	lin_pot = std::make_unique<sensor::SLS1322>(hadc1);
//
	std::unique_ptr<sensor::IAccelerometer> accelerometer(nullptr);
	accelerometer = std::make_unique<sensor::LSM6DSOX>(hi2c1);
	accelerometer->init();
	static_cast<sensor::LSM6DSOX*>(accelerometer.get())->SetODR(sensor::LSM6DSOX::SensorConfiguration::ODR12_5);
	static_cast<sensor::LSM6DSOX*>(accelerometer.get())->SetFSR(sensor::LSM6DSOX::SensorConfiguration::FSR4g);
//
//	std::unique_ptr<sensor::IGyroscope> gyroscope(nullptr);
//	gyroscope = std::make_unique<sensor::L3GD20H>(hi2c1);


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

//	float displacement_inches = 0.0f;
	float* acc_data;
//	int16_t *gyro_data = 0;

	//NVIC_SetPriorityGrouping( 0 ); //TODO
	//osKernelInitialize();	// Initialize scheduler
	//RtosInit();				// Initialize thread
	//osKernelStart();		// Start scheduler

	HAL_TIM_Base_Start_IT(&htim7);

	for(;;) {
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
//
//		displacement_inches = lin_pot->DisplacementInches();
//		printf("\n Percentage: %f", displacement_inches);
//
		accelerometer->ComputeAcceleration();
		acc_data = accelerometer->GetAcceleration();
//
		printf("the x-axis is %lf \t\t " , acc_data[0]);
		printf("the y-axis is %lf \t\t " , acc_data[1]);
		printf("the z-axis is %lf " , acc_data[2]);
		printf("\r");
		printf("\n");
//
//		gyro_data = gyroscope->DegreesPerSecond();
//		printf("x = %hd\t",gyro_data[0]);
//		printf("y = %hd\t",gyro_data[1]);
//		printf("z = %hd\t",gyro_data[2]);
//		printf("\n");
//		printf("\r");


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


		//HAL_Delay(150);

	}
}

osThreadId_t dataLoggingTaskHandle;
const osThreadAttr_t dataLoggingTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 17,
  .priority = (osPriority_t) osPriorityNormal,
};

void RtosInit() {
//	dataLoggingTaskHandle = osThreadNew(DataLoggingThread, NULL, &dataLoggingTask_attributes);
	// creates a one-shot timer:
	/*one_shot_id = osTimerNew(one_shot_Callback, osTimerOnce, (void *)0, NULL);     // (void*)0 is passed as an argument
	                                                                               // to the callback function
	// creates a periodic timer:
	periodic_id = osTimerNew(periodic_Callback, osTimerPeriodic, (void *)5, NULL); // (void*)5 is passed as an argument
	//osTimerStart(one_shot_id, 500U);
	//osTimerStart(periodic_id, 1500U);           // to the callback function

	//start on-shot timer again
	osTimerStart(one_shot_id, 500U);*/


}


void DataLoggingThread(void *argument) {
	MX_USB_HOST_Init();

	std::unique_ptr<application::IFileSystem> file_system(nullptr);
	file_system = std::make_unique<application::FatFs>(USBHPath, USBHFatFS, USBHFile);

	for (;;) {

		if(to_log == 1) {
			printf("logging");
			file_system->Mount();
			char root_file_path[] = "/ROOTFILE.csv";
			file_system->CreateFile(root_file_path);

			char root_file_header_row[] = "LinPot1,LinPot2,AccX,AccY,AccZ\n";
			char root_file_contents[] = "2.3,2.45,2,9,200\n";
			file_system->OpenFile(root_file_path, (char*)"a");
			file_system->WriteFile(root_file_header_row);
			file_system->WriteFile(root_file_contents);
			file_system->WriteFile(root_file_contents);
			file_system->CloseFile();

			HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);
			to_log = 0;

		} else if (to_unmount == 1) {
			file_system->Unmount();
			to_unmount = 0;
		}

		HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET);
	}
}


