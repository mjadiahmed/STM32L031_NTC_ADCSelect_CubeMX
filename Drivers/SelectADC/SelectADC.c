/*
 * SelectADC.c
 *
 *  Created on: Aug 5, 2022
 *      Author: mjadi
 */

#include "SelectADC.h"



uint32_t GET_ADC(uint32_t Channel) {
	uint32_t adc_conv_var;
	extern ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef adcConf;

	__HAL_RCC_ADC1_CLK_ENABLE();

	// Await the the Vrefint used by adc is set
	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VREFINTRDY) == RESET) {
	};

	// Deselect all channels
	adcConf.Channel = ADC_CHANNEL_MASK;
	adcConf.Rank = ADC_RANK_NONE;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	// Configure adc channel
	adcConf.Channel = Channel;
	adcConf.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	// Calibrate ADC
	HAL_ADC_Stop(&hadc);
	HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);

	// Start converison
	HAL_ADC_Start(&hadc);

	// Waiting for the end of conversion
	HAL_ADC_PollForConversion(&hadc, 100); // overtime 20ms

	// Read result
	adc_conv_var = HAL_ADC_GetValue(&hadc);

	// Stop
	HAL_ADC_Stop(&hadc);

	// Deselect all channels
	adcConf.Channel = ADC_CHANNEL_MASK;
	adcConf.Rank = ADC_RANK_NONE;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	__HAL_RCC_ADC1_CLK_DISABLE();
	return adc_conv_var;
}



uint32_t GET_ADC_AVG(uint32_t Channel, uint32_t times) {
	uint32_t adc_conv_var;
	uint64_t adc_acc = 0;
	extern ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef adcConf;
	HAL_StatusTypeDef adc_polling_status;
	uint32_t i;

	__HAL_RCC_ADC1_CLK_ENABLE();

	// Await Vrefint used by adc is set
	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VREFINTRDY) == RESET) {
	};

	// Deselect all channels
	adcConf.Channel = ADC_CHANNEL_MASK;
	adcConf.Rank = ADC_RANK_NONE;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	// Configure adc channel
	adcConf.Channel = Channel;
	adcConf.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	// Calibrate ADC
	HAL_ADC_Stop(&hadc);
	HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);


	// Start conversion
	HAL_ADC_Start(&hadc);

	for (i = 0; i < times;) {
		// Waiting for conversion end
		adc_polling_status = HAL_ADC_PollForConversion(&hadc, 60); // overtime 20ms

		if (adc_polling_status == HAL_OK) {
			// read data
			adc_acc += (uint64_t) HAL_ADC_GetValue(&hadc);
			i++;
		}
	}
	// Stop
	HAL_ADC_Stop(&hadc);

	// Deselect all channels
	adcConf.Channel = ADC_CHANNEL_MASK;
	adcConf.Rank = ADC_RANK_NONE;
	HAL_ADC_ConfigChannel(&hadc, &adcConf);

	__HAL_RCC_ADC1_CLK_DISABLE();

	adc_conv_var = adc_acc / times;
	return adc_conv_var;
}

