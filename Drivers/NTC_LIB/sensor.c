/*
 * sensor.c
 *
 *  Created on: Dec 20, 2021
 *      Author: NEXTRONIC
 */
#include <sensor.h>
#include <ntc_table.h>
#include "SelectADC.h"


float sensor_temperature(void){
	float adc_raw;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	 HAL_Delay(100);
//	 HAL_ADC_Start(&hadc);
//	 HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
	 adc_raw = GET_ADC_AVG(ADC_CHANNEL_6, 60);

	 //	 adc_raw= HAL_ADC_GetValue(&hadc);
	// HAL_GPIO_WritePin(NTCVCC_GPIO_Port, NTCVCC_Pin, GPIO_PIN_RESET);

	 float Ntc_Tmp_Raw = calc_temperature(adc_raw);
	 return 0.1*Ntc_Tmp_Raw;
}

