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

#ifndef LINEAR_POTENTOMETER_SLS1322_H
#define LINEAR_POTENTOMETER_SLS1322_H

// ST HAL Dependencies
#include "adc.h"

// DFR Custom Dependencies
#include "ilinear_potentiometer.hpp"


namespace sensor {

class SLS1322 : public ILinearPotentiometer {
    public:
		SLS1322();

        virtual ~SLS1322();
        
        /// @param hadc An ADC peripheral from ST's HAL.
		SLS1322(ADC_HandleTypeDef &hadc);
        

        /// Converts the ADC signal to the measured displacement of the potentiometer.
        /// @return 0.0 to 3.0 inches.
        void DisplacementInches(float quantized_counts[]) override;

        /// Converts the ADC signal to the measured displacement of the potentiometer.
        /// @return 0.0 to 76.2 millimeters.
        void DisplacementMillimeters(float quantized_counts[]) override;

    private:
        /// @brief Represents the quantized input as a ratio.
        void DisplacementRatio();

        /// Resolution for a 12-bit ADC, 2^(12 bits) - 1
        static constexpr uint16_t kMaxResolution = 0x0FFF;
        static constexpr uint8_t kMaxLengthInches = 3;
        static constexpr float kMaxLengthMillimeters = 25.4f * kMaxLengthInches;

        //DMA variables
        static constexpr uint32_t ADCBufSize = 4;
        uint32_t ADCBuf[ADCBufSize] = {0};

        // ratio of displacement
    	float retraction_ratio[ADCBufSize] = {0};

        /// The ADC peripheral from ST's HAL library.
        ADC_HandleTypeDef& adc_;
};

}

#endif // LINEAR_POTENTOMETER_SLS1322_H
