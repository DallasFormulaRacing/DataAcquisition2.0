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

SLS1322::SLS1322(ADC_HandleTypeDef& hadc) : adc_(hadc) {
	 HAL_ADC_Start_DMA(&adc_,ADCBuf,ADCBufSize);
}
SLS1322::~SLS1322() { }

void SLS1322::DisplacementInches(float quantized_counts[]) {
	DisplacementRatio();
	for(int channel = 0; channel < ADCBufSize; channel++){
		quantized_counts[channel] = retraction_ratio[channel] * kMaxLengthInches;

	}
}

void SLS1322::DisplacementMillimeters(float quantized_counts[]) {
	DisplacementRatio();
	for(int channel = 0; channel <ADCBufSize; channel++){
		quantized_counts[channel] = retraction_ratio[channel] * kMaxLengthMillimeters;
	}
}

void SLS1322::DisplacementRatio() {
	for(int channel = 0;channel < ADCBufSize; channel++){
		retraction_ratio[channel] = 1 - ((float)ADCBuf[channel] / (kMaxResolution));

	}
}

}
