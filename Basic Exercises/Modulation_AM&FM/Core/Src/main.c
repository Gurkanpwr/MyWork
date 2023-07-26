/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLES 1024
#define PI 3.14159
#define Size 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

uint16_t sine_value[SAMPLES];
uint16_t vout;
float frequency =5000.0f;
const float Sample_freq=100000.0f;
const float sine_tab_half_Value= 32768;
void AM_modulation(void);

#define NZEROS 3
#define NPOLES 3
#define GAIN   1.292942784e+04

static float xv[NZEROS+1], yv[NPOLES+1];


/*
#define NZEROS 4
#define NPOLES 4
#define GAIN   2.991490147e+002

static float xv[NZEROS+1], yv[NPOLES+1];
*/
uint16_t Sm[SAMPLES];
uint16_t ADCValue=0;
uint16_t buffer[Size];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint16_t sine_value[1024]={32768,32969,33170,33371,33572,33773,33974,34174,
		34375,34576,34777,34977,35178,35378,35579,35779,
		35979,36179,36379,36579,36779,36978,37177,37377,
		37575,37774,37973,38171,38369,38567,38765,38963,
		39160,39357,39554,39751,39947,40143,40339,40534,
		40729,40924,41119,41313,41507,41701,41894,42087,
		42279,42472,42663,42855,43046,43237,43427,43617,
		43807,43996,44184,44373,44560,44748,44935,45121,
		45307,45493,45678,45862,46046,46230,46413,46595,
		46777,46959,47140,47320,47500,47679,47858,48036,
		48214,48391,48567,48743,48919,49093,49267,49441,
		49613,49785,49957,50128,50298,50468,50636,50805,
		50972,51139,51305,51471,51635,51799,51963,52125,
		52287,52448,52609,52768,52927,53085,53243,53399,
		53555,53710,53864,54018,54170,54322,54473,54623,
		54773,54921,55069,55216,55362,55507,55652,55795,
		55938,56079,56220,56360,56499,56637,56775,56911,
		57047,57181,57315,57448,57579,57710,57840,57969,
		58097,58224,58350,58475,58600,58723,58845,58966,
		59087,59206,59324,59441,59558,59673,59787,59900,
		60013,60124,60234,60343,60451,60558,60664,60769,
		60873,60976,61078,61178,61278,61377,61474,61571,
		61666,61760,61853,61945,62036,62126,62215,62302,
		62389,62474,62559,62642,62724,62805,62885,62963,
		63041,63117,63192,63266,63339,63411,63482,63551,
		63620,63687,63753,63818,63881,63944,64005,64065,
		64124,64182,64238,64294,64348,64401,64453,64504,
		64553,64601,64648,64694,64739,64782,64825,64866,
		64905,64944,64981,65018,65053,65086,65119,65150,
		65180,65209,65237,65263,65289,65313,65335,65357,
		65377,65396,65414,65431,65446,65460,65473,65485,
		65496,65505,65513,65520,65525,65529,65533,65534,
		65535,65534,65533,65529,65525,65520,65513,65505,
		65496,65485,65473,65460,65446,65431,65414,65396,
		65377,65357,65335,65313,65289,65263,65237,65209,
		65180,65150,65119,65086,65053,65018,64981,64944,
		64905,64866,64825,64782,64739,64694,64648,64601,
		64553,64504,64453,64401,64348,64294,64238,64182,
		64124,64065,64005,63944,63881,63818,63753,63687,
		63620,63551,63482,63411,63339,63266,63192,63117,
		63041,62963,62885,62805,62724,62642,62559,62474,
		62389,62302,62215,62126,62036,61945,61853,61760,
		61666,61571,61474,61377,61278,61178,61078,60976,
		60873,60769,60664,60558,60451,60343,60234,60124,
		60013,59900,59787,59673,59558,59441,59324,59206,
		59087,58966,58845,58723,58600,58475,58350,58224,
		58097,57969,57840,57710,57579,57448,57315,57181,
		57047,56911,56775,56637,56499,56360,56220,56079,
		55938,55795,55652,55507,55362,55216,55069,54921,
		54773,54623,54473,54322,54170,54018,53864,53710,
		53555,53399,53243,53085,52927,52768,52609,52448,
		52287,52125,51963,51799,51635,51471,51305,51139,
		50972,50805,50636,50468,50298,50128,49957,49785,
		49613,49441,49267,49093,48919,48743,48567,48391,
		48214,48036,47858,47679,47500,47320,47140,46959,
		46777,46595,46413,46230,46046,45862,45678,45493,
		45307,45121,44935,44748,44560,44373,44184,43996,
		43807,43617,43427,43237,43046,42855,42663,42472,
		42279,42087,41894,41701,41507,41313,41119,40924,
		40729,40534,40339,40143,39947,39751,39554,39357,
		39160,38963,38765,38567,38369,38171,37973,37774,
		37575,37377,37177,36978,36779,36579,36379,36179,
		35979,35779,35579,35378,35178,34977,34777,34576,
		34375,34174,33974,33773,33572,33371,33170,32969,
		32768,32566,32365,32164,31963,31762,31561,31361,
		31160,30959,30758,30558,30357,30157,29956,29756,
		29556,29356,29156,28956,28756,28557,28358,28158,
		27960,27761,27562,27364,27166,26968,26770,26572,
		26375,26178,25981,25784,25588,25392,25196,25001,
		24806,24611,24416,24222,24028,23834,23641,23448,
		23256,23063,22872,22680,22489,22298,22108,21918,
		21728,21539,21351,21162,20975,20787,20600,20414,
		20228,20042,19857,19673,19489,19305,19122,18940,
		18758,18576,18395,18215,18035,17856,17677,17499,
		17321,17144,16968,16792,16616,16442,16268,16094,
		15922,15750,15578,15407,15237,15067,14899,14730,
		14563,14396,14230,14064,13900,13736,13572,13410,
		13248,13087,12926,12767,12608,12450,12292,12136,
		11980,11825,11671,11517,11365,11213,11062,10912,
		10762,10614,10466,10319,10173,10028,9883,9740,
		9597,9456,9315,9175,9036,8898,8760,8624,
		8488,8354,8220,8087,7956,7825,7695,7566,
		7438,7311,7185,7060,6935,6812,6690,6569,
		6448,6329,6211,6094,5977,5862,5748,5635,
		5522,5411,5301,5192,5084,4977,4871,4766,
		4662,4559,4457,4357,4257,4158,4061,3964,
		3869,3775,3682,3590,3499,3409,3320,3233,
		3146,3061,2976,2893,2811,2730,2650,2572,
		2494,2418,2343,2269,2196,2124,2053,1984,
		1915,1848,1782,1717,1654,1591,1530,1470,
		1411,1353,1297,1241,1187,1134,1082,1031,
		982,934,887,841,796,753,710,669,
		630,591,554,517,482,449,416,385,
		355,326,298,272,246,222,200,178,
		158,139,121,104,89,75,62,50,
		39,30,22,15,10,6,2,1,
		0,1,2,6,10,15,22,30,
		39,50,62,75,89,104,121,139,
		158,178,200,222,246,272,298,326,
		355,385,416,449,482,517,554,591,
		630,669,710,753,796,841,887,934,
		982,1031,1082,1134,1187,1241,1297,1353,
		1411,1470,1530,1591,1654,1717,1782,1848,
		1915,1984,2053,2124,2196,2269,2343,2418,
		2494,2572,2650,2730,2811,2893,2976,3061,
		3146,3233,3320,3409,3499,3590,3682,3775,
		3869,3964,4061,4158,4257,4357,4457,4559,
		4662,4766,4871,4977,5084,5192,5301,5411,
		5522,5635,5748,5862,5977,6094,6211,6329,
		6448,6569,6690,6812,6935,7060,7185,7311,
		7438,7566,7695,7825,7956,8087,8220,8354,
		8488,8624,8760,8898,9036,9175,9315,9456,
		9597,9740,9883,10028,10173,10319,10466,10614,
		10762,10912,11062,11213,11365,11517,11671,11825,
		11980,12136,12292,12450,12608,12767,12926,13087,
		13248,13410,13572,13736,13900,14064,14230,14396,
		14563,14730,14899,15067,15237,15407,15578,15750,
		15922,16094,16268,16442,16616,16792,16968,17144,
		17321,17499,17677,17856,18035,18215,18395,18576,
		18758,18940,19122,19305,19489,19673,19857,20042,
		20228,20414,20600,20787,20975,21162,21351,21539,
		21728,21918,22108,22298,22489,22680,22872,23063,
		23256,23448,23641,23834,24028,24222,24416,24611,
		24806,25001,25196,25392,25588,25784,25981,26178,
		26375,26572,26770,26968,27166,27364,27562,27761,
		27960,28158,28358,28557,28756,28956,29156,29356,
		29556,29756,29956,30157,30357,30558,30758,30959,
		31160,31361,31561,31762,31963,32164,32365,32566,};
float amplitude;
uint16_t modulation;
float phaseshift;

//float b[4]={0,0,0,2.48*10^(11)};
//float a[4]={1,15286.2408855541,97362150.1713269,248050213442.398};


static uint16_t filterloop(uint16_t Input)
  {

       xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3];
        xv[3] = Input / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3];
        yv[3] =   (xv[0] + xv[3]) + 3 * (xv[1] + xv[2])
                     + (  0.8067320684f * yv[0]) + ( -2.5964900213f * yv[1])
                     + (  2.7891392093f * yv[2]);
        uint16_t Output = yv[3];
/*
       xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
        xv[4] = (Input-4096/2) / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
        yv[4] =   (xv[0] + xv[4]) - 2 * xv[2]
                     + ( -0.8123692828f * yv[0]) + (  3.4173495370f * yv[1])
                     + ( -5.3968642686f * yv[2]) + (  3.7918699302f * yv[3]);
        uint16_t  Output = yv[4]+4096/2;
*/
        return Output;
  }
void DDSControl(void)
{

	static uint16_t phaseAccumultor;

	float phaseStep = SAMPLES/Sample_freq*frequency;
	//phaseStep +=phaseshift; // For FM MODULATION
	phaseAccumultor +=phaseStep;
	if (phaseAccumultor >=SAMPLES){
		phaseAccumultor -=SAMPLES;
	}
	float DACValue = (sine_value[phaseAccumultor]);
	//DACValue += sine_tab_half_Value;

	if(DACValue > 65535){
		DACValue = 65535;

	}
	if (DACValue <0){
		DACValue =0;
	}

	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_2, DAC_ALIGN_12B_L, DACValue);
	}
void AM_modulation(void){


	amplitude=(float)ADCValue/4096.f*0.5f +0.5f;
	phaseshift=((float)ADCValue/4096.f*0.5f +0.5f)*15;

}

void FM_Modulation(void){
	phaseshift=((float)ADCValue/4096.f*0.5f +0.5f)*15;

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
 // HAL_TIM_Base_Start_IT(&htim2);
  HAL_ADC_Start_IT(&hadc1 ) ;
  HAL_TIM_Base_Start(&htim2) ;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	//AM_modulation();
	//FM_Modulation();
	static uint8_t i=0;
	ADCValue=HAL_ADC_GetValue(&hadc1);

	vout=filterloop(ADCValue);
	//buffer[i++]=HAL_ADC_GetValue(&hadc1);
	//vout=ADCValue;
	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_2, DAC_ALIGN_12B_R, vout);

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim2){
	  DDSControl();
	  //filterloop();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
