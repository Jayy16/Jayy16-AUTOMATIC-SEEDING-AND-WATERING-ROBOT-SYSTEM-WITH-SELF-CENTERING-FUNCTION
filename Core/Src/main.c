/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
Banh FR: ENA B5, in1 E6, in2 E4
Banh BR: ENB B4, in3 E2, in4 E0

Banh FL: ENA B0, in1 A6, in2 A4
Banh BL: ENB B1, in3 A2, in4 A0

Distance 1: ben trai, Echo E9, Trig E7 (TRIG_L)
Distance 2: ben phai, Echo E13, Trig E11 (TRIG_R)
Distance 3: dang truoc, Echo E14, Trig B10 (TRIG_F)
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < time);
}
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // is the first value captured ?
unsigned int Distance1  = 0;
unsigned int Distance2  = 0;
unsigned int Distance3  = 0;
int a=0;
int i=0;
int state=3;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // if the interrupt source is channel1
	{
		if (Is_First_Captured==0) // neu chua co gia tri canh len
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // timestamp khi co canh len o chan echo
			Is_First_Captured = 1;  // khi gia tri chan echo len 1
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // neu da co gia tri canh len, thi doc cho canh xuong
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // timestamp cua canh xuong
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset counter

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffff - IC_Val1) + IC_Val2;
			}

			Distance2 = Difference * .034/2;
			Is_First_Captured = 0; // reset lai gia tri cac timestamp de doc tiep

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC3);
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
		{
			if (Is_First_Captured==0) // if the first value is not captured
			{
				IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
				Is_First_Captured = 1;  // set the first captured as true
				// Now change the polarity to falling edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
			}

			else if (Is_First_Captured==1)   // if the first is already captured
			{
				IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
				__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

				if (IC_Val2 > IC_Val1)
				{
					Difference = IC_Val2-IC_Val1;
				}

				else if (IC_Val1 > IC_Val2)
				{
					Difference = (0xffff - IC_Val1) + IC_Val2;
				}

				Distance1 = Difference * .034/2;
				Is_First_Captured = 0; // set it back to false

				// set polarity to rising edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
			}
		}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)  // if the interrupt source is channel1
		{
			if (Is_First_Captured==0) // if the first value is not captured
			{
				IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
				Is_First_Captured = 1;  // set the first captured as true
				// Now change the polarity to falling edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			}

			else if (Is_First_Captured==1)   // if the first is already captured
			{
				IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
				__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

				if (IC_Val2 > IC_Val1)
				{
					Difference = IC_Val2-IC_Val1;
				}

				else if (IC_Val1 > IC_Val2)
				{
					Difference = (0xffff - IC_Val1) + IC_Val2;
				}

				Distance3 = Difference * .034/2;
				Is_First_Captured = 0; // set it back to false

				// set polarity to rising edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC4);
			}
		}
}

void HCSR04_Read (void) //ben phai
{
	HAL_GPIO_WritePin(TRIG_R_GPIO_Port, TRIG_R_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_R_GPIO_Port, TRIG_R_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC3);
}
void HCSR04_Read_1 (void) //ben trai
{
	HAL_GPIO_WritePin(TRIG_L_GPIO_Port, TRIG_L_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_L_GPIO_Port, TRIG_L_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}
void HCSR04_Read_2 (void) //dang truoc
{
	HAL_GPIO_WritePin(TRIG_F_GPIO_Port, TRIG_F_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_F_GPIO_Port, TRIG_F_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC4);
}

void tien_len (void)
{

	 HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_SET);   // banh BR quay toi
	 HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A7_Pin,GPIO_PIN_SET);   //banh FR quay toi
	 HAL_GPIO_WritePin(GPIOA,A5_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_SET);   // banh FL quay toi
	 HAL_GPIO_WritePin(GPIOA,A6_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_SET);   //banh BL quay toi
	 HAL_GPIO_WritePin(GPIOA,A0_Pin,GPIO_PIN_RESET);

	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,400*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,380*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,400*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,400*0.7);
}

void lui_lai (void)
{
	 HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_RESET);   // banh BR quay lui
	 HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A7_Pin,GPIO_PIN_RESET);   //banh FR quay lui
	 HAL_GPIO_WritePin(GPIOA,A5_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_RESET);   // banh FL quay lui
	 HAL_GPIO_WritePin(GPIOA,A6_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_RESET);   //banh BL quay lui
	 HAL_GPIO_WritePin(GPIOA,A0_Pin,GPIO_PIN_SET);

	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,400*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,380*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,400*0.7);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,400*0.7);

}

void qua_trai (void)
{
	 HAL_GPIO_WritePin(GPIOA,A7_Pin,GPIO_PIN_SET);   //banh FR quay toi
	 HAL_GPIO_WritePin(GPIOA,A5_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_RESET);   // banh BR quay lui
	 HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_RESET);   // banh FL quay lui
	 HAL_GPIO_WritePin(GPIOA,A6_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_SET);   //banh BL quay toi
	 HAL_GPIO_WritePin(GPIOA,A0_Pin,GPIO_PIN_RESET);

	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,550*1.2);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,512*1.2);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,570*1.2);
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,560*1.2);

}

void qua_phai (void)
{
	 HAL_GPIO_WritePin(GPIOA,A7_Pin,GPIO_PIN_RESET);   //banh FR quay lui
	 HAL_GPIO_WritePin(GPIOA,A5_Pin,GPIO_PIN_SET);

	 HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_SET);   // banh BR quay toi
	 HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_SET);   // banh FL quay toi
	 HAL_GPIO_WritePin(GPIOA,A6_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_RESET);   //banh BL quay lui
	 HAL_GPIO_WritePin(GPIOA,A0_Pin,GPIO_PIN_SET);

	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,560*1.2);//BR
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,540*1.2);//FR
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,570*1.2);//FL
	 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,560*1.2);//BL

}

void dung_lai (void)
{
	 HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_RESET);   // banh BR quay toi
	 HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A7_Pin,GPIO_PIN_RESET);   //banh FR quay toi
	 HAL_GPIO_WritePin(GPIOA,A5_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_RESET);   // banh FL quay toi
	 HAL_GPIO_WritePin(GPIOA,A6_Pin,GPIO_PIN_RESET);

	 HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_RESET);   //banh BL quay toi
	 HAL_GPIO_WritePin(GPIOA,A0_Pin,GPIO_PIN_RESET);

}

void work_trai (void)
{

	//CCR 4 nap, CCR 2 cua, CCR3 quay
	//quay giua
	int i = 102;
	htim4.Instance->CCR3 = 102;//giua
	HAL_Delay(500);

	//nap hat
	htim4.Instance->CCR4 = 73;
	HAL_Delay(120);

	htim4.Instance->CCR4 = 83;
	HAL_Delay(3000);

	//140 quay trai
	while (i<140)
	{
		htim4.Instance->CCR3 = i;

		i++;

		HAL_Delay(80);
	}
	HAL_Delay(1000);

	//dong mo cua hat
	htim4.Instance->CCR2 = 50;
	HAL_Delay(1000);

	htim4.Instance->CCR2 = 80;
	HAL_Delay(1000);

	HAL_GPIO_WritePin(GPIOB,B7_Pin,GPIO_PIN_SET);
	HAL_Delay(400);
	HAL_GPIO_WritePin(GPIOB,B7_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);

	//quay giua
	i = 102;
	htim4.Instance->CCR3 = 102;//giua
	HAL_Delay(500);

}

void work_phai (void)
{

	//CCR 4 nap, CCR 2 cua, CCR3 quay
	//quay giua
	i = 102;
	htim4.Instance->CCR3 = 102;//giua
	HAL_Delay(500);

	//nap hat
	htim4.Instance->CCR4 = 73;
	HAL_Delay(120);

	htim4.Instance->CCR4 = 83;
	HAL_Delay(3000);

	//60 quay phai
	while (i>60)
	{
		htim4.Instance->CCR3 = i;

		i--;

		HAL_Delay(80);
	}
	HAL_Delay(1000);

	//dong mo cua hat
	htim4.Instance->CCR2 = 50;
	HAL_Delay(1000);

	htim4.Instance->CCR2 = 80;
	HAL_Delay(1000);

	HAL_GPIO_WritePin(GPIOB,B7_Pin,GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOB,B7_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);

	htim4.Instance->CCR3 = 102;
	HAL_Delay(1000);

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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);

  HAL_TIM_Base_Start(&htim3);               //Initialize stm32 timer 3
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

  HAL_TIM_Base_Start(&htim4);               //Initialize stm32 timer 3
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  switch (state)
	  {
	  case 1:
		  HCSR04_Read();
		  HAL_Delay(50);

		  HCSR04_Read_1();
		  HAL_Delay(50);

		  a = Distance2 - Distance1;
		  if (a>5) //sai so 3cm
			  qua_phai();

		  else if (a<-5)
			  qua_trai();

		  else if (a<5 || a>-5)
			  {
			  dung_lai();
			  HAL_Delay(500);
			  state = 2;
			  }
		  break;

//	  case 2:
//		  tien_len();
//		  HAL_Delay(500);
//		  dung_lai();
//		  HAL_Delay(500);
//		  HCSR04_Read_2();
//		  HAL_Delay(50);
//
//		  if(Distance3 <= 15)
//			  state = 3;
//		  else
//			  {
//			  work();
//			  state = 2;
//			  }
//
//
//		  break;

	  case 2:
		  tien_len();
		  HAL_Delay(600);
		  dung_lai();
		  HAL_Delay(500);
		  HCSR04_Read_2();
		  HAL_Delay(50);

		  if(Distance3 <= 15)
			  state = 3;
		  else
			  state = 4;
		  break;

	  case 3:
		  dung_lai();
		  HCSR04_Read();
		  HAL_Delay(50);

		  HCSR04_Read_1();
		  HAL_Delay(50);

		  i = Distance2 - Distance1;
		  if (i>8 || i<-8)
			  state = 1;
		  else
			  state = 3;
		  break;

	  case 4:
		  HCSR04_Read_1();
		  HAL_Delay(50);

		  HCSR04_Read();
		  HAL_Delay(50);

		  if(Distance1 > 8)
			  qua_trai();
		  else if(Distance1 <= 8)
		  {
			  dung_lai();
			  HAL_Delay(500);
			  work_trai();
			  state = 5;
		  }
		  break;

	  case 5:
		  HCSR04_Read();
		  HAL_Delay(50);

		  HCSR04_Read_1();
		  HAL_Delay(50);

		  if(Distance2 > 8)
			  qua_phai();
		  else if (Distance2 <= 8)
		  {
			  dung_lai();
			  HAL_Delay(500);
			  work_phai();
			  state = 3;
		  }
		  break;
	  default:
		  state = 3;


	  }


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 19;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 350;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 40-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, E2_Pin|E4_Pin|E6_Pin|TRIG_L_Pin
                          |TRIG_R_Pin|E0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A0_Pin|A1_Pin|A2_Pin|A3_Pin
                          |A4_Pin|A5_Pin|A6_Pin|A7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TRIG_F_Pin|B7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : E2_Pin E4_Pin E6_Pin TRIG_L_Pin
                           TRIG_R_Pin E0_Pin */
  GPIO_InitStruct.Pin = E2_Pin|E4_Pin|E6_Pin|TRIG_L_Pin
                          |TRIG_R_Pin|E0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : A0_Pin A1_Pin A2_Pin A3_Pin
                           A4_Pin A5_Pin A6_Pin A7_Pin */
  GPIO_InitStruct.Pin = A0_Pin|A1_Pin|A2_Pin|A3_Pin
                          |A4_Pin|A5_Pin|A6_Pin|A7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TRIG_F_Pin B7_Pin */
  GPIO_InitStruct.Pin = TRIG_F_Pin|B7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
