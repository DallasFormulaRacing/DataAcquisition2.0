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
extern char USBHPath[4];   // USBH logical drive path
extern FATFS USBHFatFS;    // File system object for USBH logical drive
extern FIL USBHFile;       // File object for USBH

#include "usb_host.h"
extern uint8_t usb_connected_observer; // USB connected/ejected interrupt


// DFR Custom Dependencies
#include "app.hpp"
#include "Application/circular_queue.hpp"
#include "Application/data_payload.hpp"
#include "Application/DataLogger/DataLogger.hpp"
#include "Application/FileSystem/fat_fs.hpp"
#include "Application/Mutex/mutex_cmsisv2.hpp"
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


void RtosInit();
void DataLoggingThread(void *argument);
void TimestampThread(void *argument);
void QueueProducingThread(void *argument);


/**************************************************************
 * 				Toggle Switch Interrupt Callback
 **************************************************************/
std::shared_ptr<platform::GpioStmF4> gpio_callback_ptr(nullptr);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	gpio_callback_ptr->InterruptCallback(GPIO_Pin);
}


/**************************************************************
 * 				CAN Bus Interrupt Callback
 **************************************************************/
std::shared_ptr<platform::BxCanStmF4> bx_can_callback_ptr(nullptr);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	bx_can_callback_ptr->ReceiveCallback();
}

using ReceiveInterruptMode = platform::BxCanStmF4::ReceiveInterruptMode;





void cppMain() {
	// Enable `printf()` using USART
	RetargetInit(&huart3);

	RtosInit();

	/*
	 * When `RtosInit()` is enabled, the rest of this function does NOT execute.
	 */



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



	for(;;) {
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
//		HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		if (pe3_ecu->NewMessageArrived()) {
			__disable_irq();
//			can_bus->DisableInterruptMode();

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
//			can_bus->EnableInterruptMode();
		}

	}
}





/**************************************************************
 * 						RTOS Mutexes
 **************************************************************/
const osMutexAttr_t queue_mutex_attributes = {
  "myThreadMutex",
  osMutexRecursive | osMutexPrioInherit,
  NULL,
  0U
};

const osMutexAttr_t data_mutex_attributes = {
  "myThreadMutex",
  osMutexRecursive | osMutexPrioInherit,
  NULL,
  0U
};

auto queue_mutex = std::make_shared<application::MutexCmsisV2>(queue_mutex_attributes);
auto data_mutex = std::make_shared<application::MutexCmsisV2>(data_mutex_attributes);


/**************************************************************
 * 					Shared Components
 **************************************************************/
static constexpr uint8_t size = 20;
application::CircularQueue<application::DataPayload> queue(size, queue_mutex);

application::DataPayload data_payload(data_mutex);

bool is_logging_flag = false;



/**************************************************************
 * 					RTOS Thread Properties
 **************************************************************/
osThreadId_t dataLoggingTaskHandle;
const osThreadAttr_t dataLoggingTask_attributes = {
  .name = "dataLoggingTask",
  .stack_size = 128 * 20,
  .priority = (osPriority_t) osPriorityHigh,
};


uint32_t timestamp_thread_flag = 0x00000001U;
osThreadId_t timestampTaskHandle;
const osThreadAttr_t timestampTask_attributes = {
  .name = "timestampTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityHigh,
};

osThreadId_t ecuTaskHandle;
const osThreadAttr_t ecuTask_attributes = {
  .name = "ecuTask",
  .stack_size = 128 * 17,
  .priority = (osPriority_t) osPriorityNormal,
};


/**************************************************************
 * 						RTOS Threads
 **************************************************************/
void DataLoggingThread(void *argument) {
	MX_USB_HOST_Init();

	auto file_system = std::make_shared<application::FatFs>(USBHPath, USBHFatFS, USBHFile);

	auto toggle_switch = std::make_shared<platform::GpioStmF4>(GPIOF, GPIO_PIN_15);
	gpio_callback_ptr = toggle_switch;

	application::DataLogger data_logger(file_system, toggle_switch, queue, usb_connected_observer, is_logging_flag);

	for (;;) {
		data_logger.Run();
		osDelay(1000);
	}
}

void TimestampThread(void *argument) {
	int count = 0;
	static constexpr float kTimeDuration = 2.0f; // seconds

	for(;;) {
		osThreadFlagsWait(timestamp_thread_flag, osFlagsWaitAny, osWaitForever);

		if (is_logging_flag) {
			count++;

			data_payload.Lock();
			data_payload.timestamp_ = count * kTimeDuration;
			printf("Time: %f seconds\n", data_payload.timestamp_);


			queue.Lock();
			if(queue.IsFull()) {
				printf("Queue is full! Data samples are being dropped...\n");
			}
			queue.Enqueue(data_payload);
			queue.Unlock();
			data_payload.Unlock();
		}
		else {
			count = 0;
		}
	}
}

void EcuThread(void *argument) {
	auto bx_can_peripheral = std::make_shared<platform::BxCanStmF4>(hcan1);
	std::shared_ptr<platform::ICan> can_bus = bx_can_peripheral;

	sensor::Pe3 pe3_ecu(can_bus);
	const std::vector<uint32_t>& can_id_list = pe3_ecu.CanIdList();

	// Subscribe to messages with PE3's CAN IDs
	for (const uint32_t& can_id : can_id_list) {
		bx_can_peripheral->ConfigureFilter((can_id >> 13), (can_id & 0x1FFF));
	}

	bx_can_peripheral->Start();

	// Configure and enable CAN message arrival interrupts
	bx_can_callback_ptr = bx_can_peripheral;
	ReceiveInterruptMode rx_interrupt_mode = ReceiveInterruptMode::kFifo0MessagePending;
	bx_can_peripheral->ConfigureReceiveCallback(rx_interrupt_mode);
	bx_can_peripheral->EnableInterruptMode();

	for(;;) {

		if (pe3_ecu.NewMessageArrived()) {
//			__disable_irq();
			can_bus->DisableInterruptMode();

			pe3_ecu.Update();
			uint32_t can_id = pe3_ecu.LatestCanId();

			data_payload.Lock();

			switch(can_id) {
			case FramePe1Id:
				data_payload.rpm_ = pe3_ecu.Rpm();
				data_payload.tps_ = pe3_ecu.Tps();
				data_payload.fuel_open_time_ = pe3_ecu.FuelOpenTime();
				data_payload.ignition_angle_ = pe3_ecu.IgnitionAngle();
				break;

			case FramePe2Id:
				data_payload.barometer_ = pe3_ecu.BarometerPressure();
				data_payload.map_ = pe3_ecu.Map();
				data_payload.lambda_ = pe3_ecu.Lambda();
				break;

			case FramePe3Id:
				data_payload.analog_inputs_.at(0) = pe3_ecu.AnalogInputVoltage(0);
				data_payload.analog_inputs_.at(1) = pe3_ecu.AnalogInputVoltage(1);
				data_payload.analog_inputs_.at(2) = pe3_ecu.AnalogInputVoltage(2);
				data_payload.analog_inputs_.at(3) = pe3_ecu.AnalogInputVoltage(3);
				break;

			case FramePe4Id:
				data_payload.analog_inputs_.at(4) = pe3_ecu.AnalogInputVoltage(4);
				data_payload.analog_inputs_.at(5) = pe3_ecu.AnalogInputVoltage(5);
				data_payload.analog_inputs_.at(6) = pe3_ecu.AnalogInputVoltage(6);
				data_payload.analog_inputs_.at(7) = pe3_ecu.AnalogInputVoltage(7);
				break;


			case FramePe6Id:
				data_payload.battery_voltage_ = pe3_ecu.BatteryVoltage();
				data_payload.air_temp_ = pe3_ecu.AirTemperature();
				data_payload.coolant_temp_ = pe3_ecu.CoolantTemperature();
				break;

			default:
				printf("Unhandled CAN ID:%" PRIu32 "\n", can_id);
			}

			data_payload.Unlock();

//			__enable_irq();
			can_bus->EnableInterruptMode();
		}


		osDelay(1000);
	}
}





void RtosInit() {
	NVIC_SetPriorityGrouping( 0 );	// For allowing hardware (not RTOS/software) interrupts while the Kernel is running
	osKernelInitialize(); 			// Initialize scheduler

	// Threads
	dataLoggingTaskHandle = osThreadNew(DataLoggingThread, NULL, &dataLoggingTask_attributes);
	timestampTaskHandle = osThreadNew(TimestampThread, NULL, &timestampTask_attributes);
	ecuTaskHandle = osThreadNew(EcuThread, NULL, &ecuTask_attributes);

	// Mutexes
	queue_mutex->Create();
	data_mutex->Create();

	// Hardware Timers
	HAL_TIM_Base_Start_IT(&htim7);

	osKernelStart(); 				// Start scheduler
}


