/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include <math.h>
#include "sensor.h"
#include "SelectADC.h"


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

/* USER CODE BEGIN PV */

//uint32_t BATTER_VALUE;

//Encryption
#define lowByte(v)   ((unsigned char) (v))
#define highByte(v)  ((unsigned char) (((unsigned int) (v)) >> 8))
char payload[7];

//USART
uint8_t data1;
char message1[256] = {0};
int msg_count1=0;
uint8_t data;
char message[256] = {0};
int msg_count=0;

//LP Sleep
//uint32_t send_factory = 0;
const uint16_t Seconds =3;

// measure
float temperature;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */



uint16_t LMIC_f2sflt16(float f)
{
	if (f <= -1.0)
		return 0xFFFF;
	else if (f >= 1.0)
		return 0x7FFF;
	else
	{
		int iExp;
		float normalValue;
		uint16_t sign;

		normalValue = frexpf(f, &iExp);

		sign = 0;
		if (normalValue < 0)
		{
			// set the "sign bit" of the result
			// and work with the absolute value of normalValue.
			sign = 0x8000;
			normalValue = -normalValue;

		}
		else


			// abs(f) is supposed to be in [0..1), so useful exp
			// is [0..-15]
			iExp += 15;
		if (iExp < 0)
			iExp = 0;

		// bit 15 is the sign
		// bits 14..11 are the exponent
		// bits 10..0 are the fraction
		// we conmpute the fraction and then decide if we need to round.
		uint16_t outputFraction = ldexpf(normalValue, 11) + 0.5;
		;

		if (outputFraction >= (1 << 11u))
		{
			// reduce output fraction
			outputFraction = 1 << 10;
			// increase exponent
			++iExp;

		}

		// check for overflow and return max instead.
		if (iExp > 15)
		{

			return 0x7FFF | sign;
		}

		uint16_t res = (uint16_t)(sign | (iExp << 11u) | outputFraction);

		return res;
	}
}

//USART----------------------------
void  UART_printf ( const  char * fmt1, ...)
{
	char buff1 [ 256 ];
	va_list args1;
	va_start(args1, fmt1);
	vsnprintf(buff1, sizeof (buff1), fmt1, args1);
	HAL_UART_Transmit (&hlpuart1, ( uint8_t *) buff1, strlen (buff1), HAL_MAX_DELAY);
	va_end(args1);
}
void  UART_printf2 ( const  char * fmt, ...)
{
	char buff [ 256 ];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buff, sizeof (buff), fmt, args);
	HAL_UART_Transmit (&huart2, ( uint8_t *) buff, strlen (buff), HAL_MAX_DELAY);
	va_end(args);
}


void clear_message()
{
	memset(message, 0, 256);
	msg_count=0;
}
void clear_message1()
{
	memset(message1, 0, 256);
	msg_count1=0;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &hlpuart1)
	{
		message[msg_count++] = data;
		if(data == '\0')
		{

			UART_printf2 (message);
			clear_message();
		}
		HAL_UART_Receive_IT(&hlpuart1, &data, 1);
	}
	else if(huart == &huart2)
	{
		message1[msg_count1++] = data1;
		if(data1 == '\0')
		{
			UART_printf (message1);

			clear_message1();
		}
		HAL_UART_Receive_IT(&huart2, &data1, 1);
	}
}

//void I2C_DeInit();
//void MX_LPUART1_UART_DeeInit();
//void MX_USART2_DeInit();

//LP
//void SystemPower_Config(void);
//void set_time (void);
//void SystemClockConfig_STOP(void);
//void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
//{
//	/* Clear Wake Up Flag */
//	send_factory++;
//}
//
//void HAL_SYSTICK_Callback(void)
//{
//	HAL_IncTick();
//}



//MAIN Acquisition
void EXECUTION(){
//	SystemClock_Config();
//	MX_GPIO_Init();
//	MX_LPUART1_UART_Init();
//	MX_I2C1_Init();
//	MX_USART2_UART_Init();
//	MX_ADC_Init();

	HAL_UART_Receive_IT(&hlpuart1, &data, 1);
	HAL_UART_Receive_IT(&huart2, &data1, 1);

//	//UART_printf2("wake number : %d\r\n",(send_factory));// /4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

	HAL_Delay(50);
//	bat_level();
//	//UART_printf2("\n\n ADC: %d   ||   Pourcentage: %d   ||   Volts: %.1u.%.3u V\r\n\n",BATTER_VALUE, lvlprcnt,(uint8_t)(voltage),(uint32_t)(voltage*1000)%1000);

	//response time of BME280 is 100ms
	HAL_Delay(100);
//	UART_printf ("TTM:MAC-?");


	//Measuring

//	read_bme_forced();

//	BATTER_VALUE = GET_ADC_AVG(ADC_CHANNEL_6, 60);



	temperature = sensor_temperature();
   	UART_printf2("\n Temperature NTC:  %d.%2u *C \r\n",(int) temperature,(uint32_t)(temperature*100)%100);


	// encrypting
	uint16_t payloadTemp = LMIC_f2sflt16((temperature)/100);
	payload[0] = lowByte(payloadTemp);
	payload[1] = highByte(payloadTemp);

//	uint16_t payloadHum = LMIC_f2sflt16((humidity)/100);
	payload[2] = 0x00; //lowByte(payloadHum);
	payload[3] = 0x00; // highByte(payloadHum);

//	uint16_t payloadPress = LMIC_f2sflt16((pressure)/1000000);
	payload[4] = 0x00 ; //lowByte(payloadPress);
	payload[5] = 0x00;  //highByte(payloadPress);

	payload[6] = 0x00;//(uint16_t)lvlprcnt ;
	//UART_printf2("BME280>> T: %d.%u || H:%u.%u || P: %u \r\n",(int32_t)temperature,(int32_t)(temperature*100)%100,(int32_t)humidity,(int32_t)(humidity*1000)%1000, (uint32_t) pressure );
	UART_printf2("TTM:ADD-%.2x%.2x%.2x%.2x%.2x%.2x%.2x\r\n",payload[0],payload[1],payload[2],payload[3],payload[4],payload[5],(uint16_t)payload[6]);

	UART_printf ("TTM:ADD-%.2x%.2x%.2x%.2x%.2x%.2x%.2x",payload[0],payload[1],payload[2],payload[3],payload[4],payload[5],payload[6]);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);   //LEDs
	HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//	HAL_Delay(500);
	//	HAL_Delay(100);

//	I2C_DeInit();
//	MX_USART2_DeInit();
//	MX_LPUART1_UART_DeeInit();
//	//ADC_DeInit();
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_ADC_Init();
  MX_LPUART1_UART_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */


	// Init peripheral ----------------------------------------

  	HAL_UART_Receive_IT(&hlpuart1, &data, 1);
  	HAL_UART_Receive_IT(&huart2, &data1, 1);

  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  	//LED
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  	HAL_Delay(500);
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
  	HAL_Delay(500);
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  	HAL_Delay(500);
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
  	HAL_Delay(500);


  	UART_printf ("TTM:REN-NXT_ASSET_IOT");
  	HAL_Delay(500);
  	UART_printf ("TTM:MOD-0");
  	HAL_Delay(500);
  	UART_printf ("TTM:ADP-1");
  	HAL_Delay(500);
  	UART_printf ("TTM:MAC-?");
  	HAL_Delay(500);
  	UART_printf ("TTM:RST-SYS");
  	HAL_Delay(1000);


//  	init_bme();
//  	read_bme_forced();
//  	bat_level();

  	HAL_Delay(100);

//
//  	/*
//  	 * Configure LOW PO
//  	 */
//  	I2C_DeInit();
//  	MX_USART2_DeInit();
//  	MX_LPUART1_UART_DeeInit();
//  	SystemPower_Config();
//  	hrtc.Instance = RTC;
//  	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
//  	hrtc.Init.AsynchPrediv = 127;
//  	hrtc.Init.SynchPrediv = 255;
//  	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
//  	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
//  	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//  	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
//  	if (HAL_RTC_Init(&hrtc) != HAL_OK)
//  	{
//  		Error_Handler();
//  	}
//
//  	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F){
//  		set_time();
//  	}



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		// 4 == periodicity of 32 sec (4 = 4*32 == 2 min)


//		if(send_factory % 1 == 0){


		EXECUTION();


//		temperature = sensor_temperature();
//	   	UART_printf2("\n Temperature NTC:  %d.%2u *C \r\n",(int) temperature,(uint32_t)(temperature*100)%100);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_Delay(500);


//	}

//		HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
//	//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x000ff, RTC_WAKEUPCLOCK_RTCCLK_DIV16); //0xfffff
//
//		HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS); //Seconds*1.224    // secondes*1.224
//
//		/* Enter Stop Mode */
//		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);





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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void SystemPower_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable Ultra low power mode */
	HAL_PWREx_EnableUltraLowPower();

	/* Enable the fast wake up from Ultra low power mode */
	HAL_PWREx_EnableFastWakeUp();

	/* Select HSI as system clock source after Wake Up from Stop mode */
	__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();


	/* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();


	/* Configure RTC */

}



void set_time (void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	sTime.Hours = 0x11; // set hours
	sTime.Minutes = 0x25; // set minutes
	sTime.Seconds = 0x30; // set seconds
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY   ; //   day
	sDate.Month = RTC_MONTH_DECEMBER; //  month
	sDate.Date = 0x09; // date
	sDate.Year = 0x21; // year
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
}

void I2C_DeInit(){
	HAL_I2C_DeInit(&hi2c1);
	HAL_I2C_MspDeInit(&hi2c1);
}

void MX_LPUART1_UART_DeeInit(){
	HAL_UART_DeInit(&hlpuart1);
	 HAL_UART_MspDeInit(&hlpuart1);
}

void MX_USART2_DeInit(){
	HAL_UART_DeInit(&huart2);
	HAL_UART_MspDeInit(&huart2)	;
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
