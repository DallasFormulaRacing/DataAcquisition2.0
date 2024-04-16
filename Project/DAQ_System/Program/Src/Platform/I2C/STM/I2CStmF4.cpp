/*
 * i2cHalWrapper_stmf4.cpp
 *
 *  Created on: Mar 4, 2024
 *      Author: Nico
 */
#include <Src/Platform/I2C/STM/I2CStmF4.hpp>

namespace platform {

	I2CStmF4::I2CStmF4(I2C_HandleTypeDef &hi2c)
			: i2c_(hi2c) {
	}

	I2CStmF4::~I2CStmF4(){}

	void I2CStmF4::Receive(uint8_t *response, uint8_t dev_add_, uint8_t response_size_){
		HAL_I2C_Master_Receive(&i2c_, dev_add_, response, response_size_, HAL_MAX_DELAY);
	}

	void I2CStmF4::Transmit(uint8_t *commands, uint8_t dev_add_, uint8_t maxBytes){
		HAL_I2C_Master_Transmit(&i2c_,dev_add_, commands, maxBytes, HAL_MAX_DELAY);
	}

}

