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
uint16_t adc_value[2];  // 0 - 0 channel; 1 - temperature
TimerHandle_t Timer;
void vTimerCallback( TimerHandle_t Timer );
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef* adcHandle;
/* USER CODE END Variables */
osThreadId ADC_taskHandle;
osThreadId UART_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void start_ADC_task(void const * argument);
void start_UART_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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
  /* definition and creation of ADC_task */
  osThreadDef(ADC_task, start_ADC_task, osPriorityNormal, 0, 256);
  ADC_taskHandle = osThreadCreate(osThread(ADC_task), NULL);

  /* definition and creation of UART_task */
  osThreadDef(UART_task, start_UART_task, osPriorityNormal, 0, 128);
  UART_taskHandle = osThreadCreate(osThread(UART_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_start_ADC_task */
/**
  * @brief  Function implementing the ADC_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_ADC_task */
void start_ADC_task(void const * argument)
{
  /* USER CODE BEGIN start_ADC_task */
	Timer = xTimerCreate("Timer", 500, pdTRUE, ( void * ) 0, vTimerCallback);
	                     /* The ID is used to store a count of the
	                     number of times the timer has expired, which
	                     is initialised to 0. */
	                     // ( void * ) 0,
	xTimerStart(Timer, 0);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END start_ADC_task */
}

/* USER CODE BEGIN Header_start_UART_task */
/**
* @brief Function implementing the UART_task thread.
* @param argument: Not used
* @retval None
*/
char c[16] = {};
extern UART_HandleTypeDef huart2;

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
/* USER CODE END Header_start_UART_task */
void start_UART_task(void const * argument)
{
  /* USER CODE BEGIN start_UART_task */
  /* Infinite loop */
	for(int i = 0;;i++)
	{
		osDelay(500);
		HAL_GPIO_TogglePin(LD_GPIO_Port, LD_Pin);
		itoa(i%10, c, 10);
		printf("%c\n",c[0]);
	}
  /* USER CODE END start_UART_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vTimerCallback( TimerHandle_t Timer )
 {
// const uint32_t ulMaxExpiryCountBeforeStopping = 10;
 uint32_t ulCount;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT( Timer );

    /* The number of times this timer has expired is saved as the
    timer's ID.  Obtain the count. */
    ulCount = ( uint32_t ) pvTimerGetTimerID( Timer );

    /* Increment the count, then test to see if the timer has expired
    ulMaxExpiryCountBeforeStopping yet. */
    ulCount++;

//    /* If the timer has expired 10 times then stop it from running. */
//    if( ulCount >= ulMaxExpiryCountBeforeStopping )
//    {
//        /* Do not use a block time if calling a timer API function
//        from a timer callback function, as doing so could cause a
//        deadlock! */
//        xTimerStop( Timer, 0 );
//    }
//    else
//    {
//       /* Store the incremented count back into the timer's ID field
//       so it can be read back again the next time this software timer
//       expires. */
//       vTimerSetTimerID( Timer, ( void * ) ulCount );
//    }
    HAL_GPIO_TogglePin(LD_GPIO_Port, LD_Pin);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
	// adc_value[0] = (uint16_t)HAL_ADC_GetValue(&hadc1);
	adc_value[1] = (uint16_t)HAL_ADC_GetValue(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, 10);
	adc_value[0]=(uint16_t)HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);
 }
/* USER CODE END Application */
