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
uint16_t period = 100;
GPIO_PinState pin_state;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId led_taskHandle;
osThreadId button_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void start_led_task(void const * argument);
void start_button_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of led_task */
  osThreadDef(led_task, start_led_task, osPriorityNormal, 0, 128);
  led_taskHandle = osThreadCreate(osThread(led_task), NULL);

  /* definition and creation of button_task */
  osThreadDef(button_task, start_button_task, osPriorityNormal, 0, 128);
  button_taskHandle = osThreadCreate(osThread(button_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
	  osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_start_led_task */
/**
* @brief Function implementing the led_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_led_task */
void start_led_task(void const * argument)
{
  /* USER CODE BEGIN start_led_task */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LD_GPIO_Port, LD_Pin);
	  osDelay(period);
  }
  /* USER CODE END start_led_task */
}

/* USER CODE BEGIN Header_start_button_task */
/**
* @brief Function implementing the button_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_button_task */
void start_button_task(void const * argument)
{
  /* USER CODE BEGIN start_button_task */
  /* Infinite loop */
  for(;;)
  {
    if (pin_state==GPIO_PIN_SET) {
      period += 100;
      pin_state = GPIO_PIN_RESET;
    }
    if (period > 1000) {
      period = 100;
    }
	  osDelay(1000);
  }
  /* USER CODE END start_button_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == btn_Pin) {
		pin_state = HAL_GPIO_ReadPin(btn_GPIO_Port, btn_Pin);
	}
	if(GPIO_Pin == pin2_Pin) {
		if(HAL_GPIO_ReadPin(pin2_GPIO_Port, pin2_Pin) == GPIO_PIN_RESET)
			period = 100;
	}
}
/* USER CODE END Application */
