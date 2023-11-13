/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

#include <stdio.h>
#include <inttypes.h>
#include "retarget.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t Task1Handle;
const osThreadAttr_t Task1_attributes = {
	.name = "Task1",
	.stack_size = 128 * 4,
	.priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t Task2Handle;
const osThreadAttr_t Task2_attributes = {
	.name = "Task2",
	.stack_size = 128 * 4,
	.priority = (osPriority_t) osPriorityAboveNormal,
};

osThreadId_t Task3Handle;
const osThreadAttr_t Task3_attributes = {
	.name = "Task3",
	.stack_size = 128 * 4,
	.priority = (osPriority_t) osPriorityBelowNormal,
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Task1_init(void *argument);
void Task2_init(void *argument);
void Task3_init(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	Task1Handle = osThreadNew(Task1_init, NULL, &Task1_attributes);
	//Task2Handle = osThreadNew(Task2_init, NULL, &Task2_attributes);
	//Task3Handle = osThreadNew(Task3_init, NULL, &Task3_attributes);

	// Avoid initializing the auto-generated default task
	return;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Task1_init(void *argument) {
	RetargetInit(&huart3);
	uint32_t value_adc = 0;

	while (true) {
//		uint8_t data[] = "Hello TASK1\n";
//		HAL_UART_Transmit(&huart3, data, 12, 1000);
//		osDelay(1000);
		printf("\r\nADC Value: ");
		printf("%" PRIu32 "\n", value_adc);

		HAL_Delay(100);
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		value_adc = HAL_ADC_GetValue(&hadc1);


	}
}

//void Task2_init(void *argument) {
//	while (true) {
//		uint8_t data[] = "Hello TASK2\n";
//		HAL_UART_Transmit(&huart3, data, 12, 1000);
//		osDelay(1000);
//	}
//}
//
//void Task3_init(void *argument) {
//	while (true) {
//		uint8_t data[] = "Hello TASK3\n";
//		HAL_UART_Transmit(&huart3, data, 12, 1000);
//		osDelay(1000);
//	}
//}
/* USER CODE END Application */

