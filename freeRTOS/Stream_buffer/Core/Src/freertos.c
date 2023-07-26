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
#include "stream_buffer.h"

#include "string.h"
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
//float res=(3.3)/4096;
StreamBufferHandle_t xStreamBuffer;
const size_t xStreamBufferSizeBytes = 100, xTriggerLevel = 1;

uint16_t adc_value = 0;
const uint8_t threshold = 40;
char message[40];

uint8_t ucRxData[ 40 ];
size_t xReceivedBytes;

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
/* USER CODE END Variables */
/* Definitions for monitor_task */
osThreadId_t monitor_taskHandle;
const osThreadAttr_t monitor_task_attributes = {
  .name = "monitor_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensor1_task */
osThreadId_t sensor1_taskHandle;
const osThreadAttr_t sensor1_task_attributes = {
  .name = "sensor1_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for sensor2_task */
osThreadId_t sensor2_taskHandle;
const osThreadAttr_t sensor2_task_attributes = {
  .name = "sensor2_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void start_monitor_task(void *argument);
void start_sensor1_task(void *argument);
void start_sensor2_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
//	HAL_ADC_Start_IT(&hadc1);
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
  /* creation of monitor_task */
  monitor_taskHandle = osThreadNew(start_monitor_task, NULL, &monitor_task_attributes);

  /* creation of sensor1_task */
  sensor1_taskHandle = osThreadNew(start_sensor1_task, NULL, &sensor1_task_attributes);

  /* creation of sensor2_task */
  sensor2_taskHandle = osThreadNew(start_sensor2_task, NULL, &sensor2_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xStreamBuffer = xStreamBufferCreate( xStreamBufferSizeBytes, xTriggerLevel );
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_start_monitor_task */
/**
  * @brief  Function implementing the monitor_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_monitor_task */
void start_monitor_task(void *argument)
{
  /* USER CODE BEGIN start_monitor_task */
//	HAL_ADC_Start_IT(&hadc1);
	const TickType_t xBlockTime = pdMS_TO_TICKS( 20 );

  /* Infinite loop */
  for(;;)
  {
	xReceivedBytes= xStreamBufferReceive(xStreamBuffer, (void*)ucRxData, 40, xBlockTime);
	HAL_UART_Transmit(&huart2, (void*)ucRxData, 40, 1000);
    osDelay(1000);
  }
  /* USER CODE END start_monitor_task */
}

/* USER CODE BEGIN Header_start_sensor1_task */
/**
* @brief Function implementing the sensor1_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_sensor1_task */
void start_sensor1_task(void *argument)
{
  /* USER CODE BEGIN start_sensor1_task */
  /* Infinite loop */
  for(;;)
  {

    osDelay(1000);
  }
  /* USER CODE END start_sensor1_task */
}

/* USER CODE BEGIN Header_start_sensor2_task */
/**
* @brief Function implementing the sensor2_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_sensor2_task */
void start_sensor2_task(void *argument)
{
  /* USER CODE BEGIN start_sensor2_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END start_sensor2_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; /* Initialised to pdFALSE. */
	size_t xBytesSent;

	if (hadc->Instance == ADC1) {
		adc_value = HAL_ADC_GetValue(&hadc1);//*res;
		if (adc_value > threshold) {
			sprintf(message,"Our value= %d/n/r",adc_value);
		    xBytesSent = xStreamBufferSendFromISR( xStreamBuffer,
		                                           ( void * ) message,
		                                           strlen( message ),
		                                           &xHigherPriorityTaskWoken );
		  if(xBytesSent!=strlen( message ) ){
			  //Not fully sent
		  }
//		  taskYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		}
	}
}
/* USER CODE END Application */

