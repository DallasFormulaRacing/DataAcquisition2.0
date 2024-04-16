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

// 3rd Party Libraries and Frameworks
#include "cmsis_os.h"

#include "fatfs.h"
extern char USBHPath[4];   // USBH logical drive path
extern FATFS USBHFatFS;    // File system object for USBH logical drive
extern FIL USBHFile;       // File object for USBH

#include "usb_host.h"
extern uint8_t usb_connected_observer; // USB connected/ejected interrupt


// DFR Custom Dependencies
#include "app.hpp"
#include "Application/data_payload.hpp"
#include "Application/DataLogger/DataLogger.hpp"
#include "Application/FileSystem/fat_fs.hpp"
#include "Platform/CAN/STM/F4/bxcan_stmf4.hpp"
#include "Platform/CAN/Interfaces/ican.hpp"
#include "Platform/GPIO/igpio.hpp"
#include "Platform/GPIO/gpio_stmf4.hpp"
#include "Sensor/Accelerometer/lsm303dlhc.hpp"
#include "Sensor/ECU/PE3/iecu.hpp"
#include "Sensor/ECU/PE3/pe3.hpp"
#include "Sensor/GyroScope/igyroscope.hpp"
#include "Sensor/GyroScope/l3gd20h.hpp"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"
#include "Sensor/Accelerometer/LSM6DSOXAccelerometer.hpp"
#include "../../Core/Inc/retarget.h"



// Toggle Switch Interrupt Callback
std::shared_ptr<platform::GpioStmF4> gpio_callback_ptr(nullptr);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	gpio_callback_ptr->InterruptCallback(GPIO_Pin);
}


#include "Application/circular_queue.hpp"
#include "Application/Mutex/mutex_cmsisv2.hpp"

// CAN Bus Interrupt Callback
std::shared_ptr<platform::BxCanStmF4> bx_can_callback_ptr(nullptr);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	bx_can_callback_ptr->ReceiveCallback();
}

using ReceiveInterruptMode = platform::BxCanStmF4::ReceiveInterruptMode;




void StartFreeRtos();
void RtosInit();
void DataLoggingThread(void *argument);



void cppMain() {
	// Enable `printf()` using USART
	RetargetInit(&huart3);


	auto bx_can_peripheral = std::make_shared<platform::BxCanStmF4>(hcan1);
	std::shared_ptr<platform::ICan> can_bus = bx_can_peripheral;

	auto pe3_ecu = std::make_unique<sensor::Pe3>(can_bus);
	const std::vector<uint32_t>& can_id_list = pe3_ecu->CanIdList();

	auto bx_i2c_peripheral = std::make_shared<platform::I2CStmF4>(hi2c1);
	std::shared_ptr<platform::II2C> i2c_line = bx_i2c_peripheral;

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

	RtosInit();

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





	}
}

osThreadId_t dataLoggingTaskHandle;
const osThreadAttr_t dataLoggingTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 20,
  .priority = (osPriority_t) osPriorityHigh,
};



void QueueProducingThread(void *argument);
void QueueConsumingThread(void *argument);

osThreadId_t producerTaskHandle;
const osThreadAttr_t producerTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 17,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t consumerTaskHandle;
const osThreadAttr_t consumerTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 17,
  .priority = (osPriority_t) osPriorityHigh,
};



const osMutexAttr_t queue_thread_attributes = {
  "myThreadMutex",                          // human readable mutex name
  osMutexRecursive | osMutexPrioInherit,    // attr_bits
  NULL,                                     // memory for control block
  0U                                        // size for control block
};

auto wrapper_mutex = std::make_shared<application::MutexCmsisV2>(queue_thread_attributes);
uint8_t size = 20;
auto q = std::make_shared<application::CircularQueue<application::DataPayload>>(size, wrapper_mutex);

void RtosInit() {
	NVIC_SetPriorityGrouping( 0 );	// For allowing hardware (not RTOS/software) interrupts while the Kernel is running
	osKernelInitialize(); 			// Initialize scheduler

	dataLoggingTaskHandle = osThreadNew(DataLoggingThread, NULL, &dataLoggingTask_attributes);
	producerTaskHandle = osThreadNew(QueueProducingThread, NULL, &producerTask_attributes);
//	consumerTaskHandle = osThreadNew(QueueConsumingThread, NULL, &consumerTask_attributes);

	wrapper_mutex->Create();

	osKernelStart(); 				// Start scheduler
}







void QueueProducingThread(void *argument) {
	application::DataPayload data;

	data.timestamp_ = 1;
	data.linpot_displacement_inches_[0] = 2.5;
	data.linpot_displacement_inches_[1] = 0.5;
	data.linpot_displacement_inches_[2] = 1.3;
	data.linpot_displacement_inches_[3] = 4.0;

	for(;;) {
		q->Lock();
		q->Enqueue(data);
		q->Unlock();

		data.timestamp_*= 2;
		data.linpot_displacement_inches_[0] *= 2;
		data.linpot_displacement_inches_[1] *= 2;
		data.linpot_displacement_inches_[2] *= 2;
		data.linpot_displacement_inches_[3] *= 2;

		osDelay(1000);
	}
}

void QueueConsumingThread(void *argument) {
	application::DataPayload received_data;

	for(;;) {
		q->Lock();

		if(!q->IsEmpty()) {
			received_data = q->Dequeue();
			printf("\nReceived: %d", received_data.timestamp_);
		}

		q->Unlock();
		osDelay(300);
	}
}





void DataLoggingThread(void *argument) {
	MX_USB_HOST_Init();

	std::shared_ptr<application::IFileSystem> file_system(nullptr);
	file_system = std::make_shared<application::FatFs>(USBHPath, USBHFatFS, USBHFile);

	auto switch_gpio_peripheral = std::make_shared<platform::GpioStmF4>(GPIOF, GPIO_PIN_15);
	std::shared_ptr<platform::IGpio> toggle_switch = switch_gpio_peripheral;
	gpio_callback_ptr = switch_gpio_peripheral;

	application::DataLogger data_logger(file_system, toggle_switch, &usb_connected_observer, q);

	for (;;) {
		data_logger.Run();
		osDelay(2300);
	}
}


