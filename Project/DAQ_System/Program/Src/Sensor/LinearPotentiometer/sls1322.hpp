/*
* Linear Potentiometer SLS1300
* Author:        Manuel DJC
* Modified By:   Cristian Cruz
*
* Email:         Manuel.DeJesusContreras@UTDallas.edu
*                cris14.alex@gmail.com
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

/// SLS1322
class SLS1322 : public ILinearPotentiometer {
    public:
		SLS1322();
		SLS1322(ADC_HandleTypeDef& hadc);
        virtual ~SLS1322();

        /// DisplacementInches 
        /// @param None
        /// @return Displacement as inches HAHAHA HUHH?!
        float DisplacementInches() override;
        float DisplacementMillimeters() override;

    private:
        // Returns an ADC reading after the sampling undergoes quantization.
		// Range: 0 to ADC resolution
		//		 That is, 0 to 2^(n bits)-1
		uint32_t ReadQuantizedInput();
        // Returns an ADC reading within a range of 0.0 to 1.0.
        float DisplacementRatio();

        ADC_HandleTypeDef& adc_;

        static constexpr uint16_t kMaxResolution = 0x0FFF; // 2^(12 bits) - 1
        static constexpr uint8_t kMaxLengthInches = 3;
        static constexpr float kMaxLengthMillimeters = 25.4f * kMaxLengthInches;
};

}

#endif // LINEAR_POTENTOMETER_SLS1322_H
