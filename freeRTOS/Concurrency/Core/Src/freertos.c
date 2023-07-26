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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct data_t {
	float mean;
	float mean_square;
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_SAMPLES 1000
#define NUM_OF_TASKS 2
#define BUF_SZIE 50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
struct data_t task_1_result, task_2_result;
SemaphoreHandle_t mutex,semaphore_handle;
/* USER CODE END Variables */
osThreadId task_1Handle;
osThreadId task_2Handle;
osThreadId UART_taskHandle;
osSemaphoreId semaHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void start_task_1(void const * argument);
void start_task_2(void const * argument);
void start_UART_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

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

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
//	mutex=xSemaphoreCreateMutex();
	mutex= xSemaphoreCreateBinary();
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of sema */
  osSemaphoreDef(sema);
  semaHandle = osSemaphoreCreate(osSemaphore(sema), 1);

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
  /* definition and creation of task_1 */
  osThreadDef(task_1, start_task_1, osPriorityLow, 0, 2000);
  task_1Handle = osThreadCreate(osThread(task_1), NULL);

  /* definition and creation of task_2 */
  osThreadDef(task_2, start_task_2, osPriorityNormal, 0, 2000);
  task_2Handle = osThreadCreate(osThread(task_2), NULL);

  /* definition and creation of UART_task */
  osThreadDef(UART_task, start_UART_task, osPriorityIdle, 0, 1024);
  UART_taskHandle = osThreadCreate(osThread(UART_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_start_task_1 */
/**
  * @brief  Function implementing the task_1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_task_1 */
void start_task_1(void const * argument)
{
  /* USER CODE BEGIN start_task_1 */
	float array[NUM_SAMPLES]; //, mean, mean_square;
//	for(uint16_t i =0; i < NUM_SAMPLES; i++) {
//		array[i] = 0;
//	}
	float sum=0, sum_ofsquares=0;
  /* Infinite loop */
  for(;;)
  {
	  if(xSemaphoreTake(mutex,(TickType_t )10)==pdTRUE){
	  for(uint16_t i =0; i < NUM_SAMPLES; i++) {

			  array[i] = ((float)rand()/RAND_MAX)*2-1;
			  sum += array[i];
			  sum_ofsquares += array[i]*array[i];
	  }



		  HAL_GPIO_TogglePin(T1_GPIO_Port, T1_Pin);

	  task_1_result.mean = sum / NUM_SAMPLES;
	  task_1_result.mean_square = sqrt(sum_ofsquares / NUM_SAMPLES);
	  xSemaphoreGive(mutex);
	  HAL_GPIO_TogglePin(T1_GPIO_Port, T1_Pin);



	  }
    osDelay(10);
  }
  /* USER CODE END start_task_1 */
}

/* USER CODE BEGIN Header_start_task_2 */
/**
* @brief Function implementing the task_2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_task_2 */
void start_task_2(void const * argument)
{
  /* USER CODE BEGIN start_task_2 */
	float array[NUM_SAMPLES];
	float sum=0, sum_ofsquares=0;
  /* Infinite loop */
  for(;;)
  {
	  if(xSemaphoreTake(mutex,(TickType_t )10)==pdTRUE){
	  for(uint16_t i =0; i < NUM_SAMPLES; i++) {

		  array[i] = ((float)rand()/RAND_MAX)*2-1;
		  sum += array[i];
		  sum_ofsquares += array[i]*array[i];
	  }



		  HAL_GPIO_TogglePin(T2_GPIO_Port, T2_Pin);

	  task_2_result.mean = sum / NUM_SAMPLES;
	  task_2_result.mean_square = sqrt(sum_ofsquares / NUM_SAMPLES);
	  xSemaphoreGive(mutex);
	  HAL_GPIO_TogglePin(T2_GPIO_Port, T2_Pin);

	  }


	  osDelay(10);
  }
  /* USER CODE END start_task_2 */
}

/* USER CODE BEGIN Header_start_UART_task */
char c[BUF_SZIE], c2[BUF_SZIE];
extern UART_HandleTypeDef huart2;

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	osDelay(10);
	return ch;
}
/**
* @brief Function implementing the UART_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_UART_task */
void start_UART_task(void const * argument)
{
  /* USER CODE BEGIN start_UART_task */
  /* Infinite loop */
struct data_t result;
  for(;;)
  {
	  if(xSemaphoreTake(mutex,(TickType_t)100)==pdTRUE){
		 osDelay(1000);
		  HAL_GPIO_TogglePin(PRINTER_GPIO_Port, PRINTER_Pin);
	  result.mean = (task_1_result.mean + task_2_result.mean) / NUM_OF_TASKS;
	  result.mean_square = (task_1_result.mean_square + task_2_result.mean_square) / NUM_OF_TASKS;
	  }

	  xSemaphoreGive(mutex);
//	  HAL_UART_Transmit(&huart2, task1_Res.mean, sizeof(task1_Res.mean), 1000);
	  HAL_GPIO_TogglePin(PRINTER_GPIO_Port, PRINTER_Pin);

//	  for (int i = 0; i < ;i++)
//		  {

	  sprintf(c, "mean of means: %.2f\n\r", result.mean);
	  puts(c);
	  sprintf(c2, "mean of mean squares: %.2f\n\r", result.mean_square);
	  puts(c2);


//	  __io_putchar(c);
//	  for (int j = 0; j < 2; j++) {  // or without this line and i instead of j
//	  			j = j%2;
//	  //			itoa(data_value.adc_value, c[j], 10);  // need to provide performing for second channel
//	  			printf("%c\n", c[j]);
//	  		}
//	  for(int i = 0; i < 26; i++)
//	  {
//		  printf("%c", c[i]);
////		  putchar('0');
////		  osDelay(100);
//	  }
//	  	  printf()
//	  putchar('\r');
//	  putchar('\n');
  }
  // "mean of means:
  // "mean of mean_squares:



  /* USER CODE END start_UART_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
