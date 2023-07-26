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
#include "adc.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart2;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct ADC_READ_STRUCT{
	uint32_t channel_no;
	float Channel_0_Value;
	uint16_t Channel_Temp_Value;
}ADC_t;

uint16_t DMA_Buffer[2];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define VREF 3.3f
#define ADC_RESOLUTION 4095.0f
#define LM35_SCALE_FACTOR 0.01f
#define TEMP_SAMPLES 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern ADC_HandleTypeDef hadc1;
float resolution = VREF / ADC_RESOLUTION;
float Temperature=0.0;
float TVoltage,CVoltage;
SemaphoreHandle_t mutex;
ADC_t Poll_Data={0};
ADC_t Glob_Data={0};
ADC_t Avg_data;

QueueHandle_t ADCtoUARTqueueHandle;	/* Queue definitions are Here...*/
QueueHandle_t ADCqueue_t;

float temp_array[TEMP_SAMPLES];
float temp_sum = 0.0f;
int temp_index = 0;
/* USER CODE END Variables */
osThreadId ADC_READ_POLLHandle;
osThreadId ADC_TaskHandle;
osThreadId UART_TaskHandle;
osMessageQId ADCqueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ADC_READ(void const * argument);
void ADC_Tasks(void const * argument);
void UART_transmission(void const * argument);

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
	 for (int i = 0; i < TEMP_SAMPLES; i++)
	    {
	        temp_array[i] = 0.0f;
	    }
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */

	mutex = xSemaphoreCreateMutex();
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of ADCqueue */
  osMessageQDef(ADCqueue, 16, uint16_t);
  ADCqueueHandle = osMessageCreate(osMessageQ(ADCqueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */

  /* add queues, ... */
  ADCqueue_t = xQueueCreate(10, sizeof(ADC_t));
  if(ADCqueue_t==NULL){
	  //Queue error;
  }
  ADCtoUARTqueueHandle = xQueueCreate(10, sizeof(ADC_t));

  if (ADCtoUARTqueueHandle == NULL) {
	  //Queue error;
  }

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ADC_READ_POLL */
  osThreadDef(ADC_READ_POLL, ADC_READ, osPriorityNormal, 0, 128);
  ADC_READ_POLLHandle = osThreadCreate(osThread(ADC_READ_POLL), NULL);

  /* definition and creation of ADC_Task */
  osThreadDef(ADC_Task, ADC_Tasks, osPriorityIdle, 0, 256);
  ADC_TaskHandle = osThreadCreate(osThread(ADC_Task), NULL);

  /* definition and creation of UART_Task */
  osThreadDef(UART_Task, UART_transmission, osPriorityNormal, 0, 128);
  UART_TaskHandle = osThreadCreate(osThread(UART_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_ADC_READ */
/**
  * @brief  Function implementing the ADC_READ_POLL thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ADC_READ */
void ADC_READ(void const * argument)
{
  /* USER CODE BEGIN ADC_READ */
  /* Infinite loop */
//	ADC_t Poll_datas;
//	BaseType_t sendResult;

  for(;;)
  {


//		ADC_Select_CH0();
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 1000);
//		Poll_datas.Channel_0_Value=HAL_ADC_GetValue(&hadc1);
//		HAL_ADC_Stop(&hadc1);
//
//		ADC_Select_CHTemp();
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 1000);
//		Poll_datas.Channel_Temp_Value=HAL_ADC_GetValue(&hadc1);
//		HAL_ADC_Stop(&hadc1);
//		sendResult = xQueueSend(ADCqueue_t, (void *)&Poll_datas, pdMS_TO_TICKS(100));
//		if(sendResult !=pdPASS){
//			Error_Handler();
//		}
		osDelay(100);

  }
  /* USER CODE END ADC_READ */
}

/* USER CODE BEGIN Header_ADC_Tasks */
/**
* @brief Function implementing the ADC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ADC_Tasks */
void ADC_Tasks(void const * argument)
{
  /* USER CODE BEGIN ADC_Tasks */
  /* Infinite loop */
	ADC_t Datas;
	uint16_t custom_pin = 0, tmps = 0;
	uint8_t  channel;
	const float V25 = 0.76f;
	const float Avg_Slope = 0.0025f;

	for (;;)
	{
		 if (xQueueReceive(ADCqueue_t, &Datas, portMAX_DELAY) == pdPASS)
			    {

					channel=Datas.channel_no;
					Avg_data.channel_no=channel;

					custom_pin=Datas.Channel_0_Value;
					CVoltage = (float)custom_pin * resolution;

					Avg_data.Channel_0_Value=CVoltage;

					tmps = Datas.Channel_Temp_Value;
					float TVoltage = (float)tmps * resolution;
					float Temperature = (TVoltage - V25) / Avg_Slope + 25;


					temp_sum = temp_sum - temp_array[temp_index] + Temperature;
					temp_array[temp_index] = Temperature;
					temp_index = (temp_index + 1) % TEMP_SAMPLES;
					float temp_avg = temp_sum / TEMP_SAMPLES;

					Avg_data.channel_no = 1;
					Avg_data.Channel_Temp_Value = temp_avg;
					BaseType_t sendResult;

					sendResult = xQueueSend(ADCtoUARTqueueHandle, (void *)&Avg_data, pdMS_TO_TICKS(100));
					if (sendResult != pdPASS) {
							// Failed to send the data to the queue
					}

			    }


	  vTaskDelay(pdMS_TO_TICKS(100));

  }
  /* USER CODE END ADC_Tasks */
}

/* USER CODE BEGIN Header_UART_transmission */
/**
* @brief Function implementing the UART_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UART_transmission */
void UART_transmission(void const * argument)
{
  /* USER CODE BEGIN UART_transmission */
  /* Infinite loop */
	 ADC_t Received_Data;
	 char Uart_Data[50]="";
	 uint8_t results;
  for(;;)
  {

	  if (xQueueReceive(ADCtoUARTqueueHandle, &Received_Data, pdMS_TO_TICKS(100)) == pdPASS) {

	      // Print the data to the terminal
		  sprintf(Uart_Data,"Channel_0 Voltage: %d\nTemperature: %d°C\n",
				  (uint16_t)Received_Data.Channel_0_Value,
				  Received_Data.Channel_Temp_Value);
		 results= HAL_UART_Transmit_IT(&huart2, (uint8_t*)Uart_Data, strlen(Uart_Data));
		 if(results != HAL_OK){
			 Error_Handler();
		 }
	  }
	  vTaskDelay(pdMS_TO_TICKS(1000));

  }
  /* USER CODE END UART_transmission */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // define xHigherPriorityTaskWoken as local variable
    if (hadc->Instance == ADC1) {
        HAL_ADC_Start_DMA(hadc, (uint32_t*) DMA_Buffer, 2);
        uint16_t* pData = DMA_Buffer;
        Glob_Data.Channel_0_Value = pData[1];
        Glob_Data.Channel_Temp_Value = pData[0];
        xQueueSendFromISR(ADCqueue_t, (void *)&Glob_Data, &xHigherPriorityTaskWoken); // send data to queue
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        // USART2 transmission complete callback

        // Do nothing in this case
    }
}


/* USER CODE END Application */
