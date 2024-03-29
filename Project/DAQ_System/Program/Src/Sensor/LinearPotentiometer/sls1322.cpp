/*
* Linear Potentiometer SLS1300
* Author:   Manuel DJC
*           Cristian Cruz
*
* Email:    Manuel.DeJesusContreras@UTDallas.edu
*           cris14.alex@gmail.com
*
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#include "sls1322.hpp"

namespace sensor {

SLS1322::SLS1322(ADC_HandleTypeDef& hadc) : adc_(hadc) { }
SLS1322::~SLS1322() { }

float SLS1322::DisplacementInches() {
	float displacement_ratio = DisplacementRatio();
	return displacement_ratio * kMaxLengthInches;
}

float SLS1322::DisplacementMillimeters() {
	float displacement_ratio = DisplacementRatio();
	return displacement_ratio * kMaxLengthMillimeters;
}

uint32_t SLS1322::ReadQuantizedInput() {
	HAL_ADC_Start(&adc_);
	HAL_ADC_PollForConversion(&adc_, 1);
	return HAL_ADC_GetValue(&adc_);
}

float SLS1322::DisplacementRatio() {
	uint32_t quantized_count = ReadQuantizedInput();
	float retraction_ratio = (float)(quantized_count) / kMaxResolution;
	return 1 - retraction_ratio;
}

}
