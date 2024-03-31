/*
 * i2cHalWrapper_stmf4.cpp
 *
 *  Created on: Mar 4, 2024
 *      Author: Nico
 */
#include "i2cHalWrapper_stmf4.hpp"

namespace platform {

	i2cHalWrapperStmf4::i2cHalWrapperStmf4(I2C_HandleTypeDef &hi2c)
			: bx_i2c_(hi2c) {
	}

	i2cHalWrapperStmf4::~i2cHalWrapperStmf4(){}

	void i2cHalWrapperStmf4::Transmit(uint8_t *commands, uint8_t dev_add_){
		uint8_t maxBytes = sizeof(&commands)/sizeof(uint8_t);
		HAL_I2C_Master_Transmit(&bx_i2c_,dev_add_, commands, maxBytes, HAL_MAX_DELAY);
	}
	void i2cHalWrapperStmf4::Receive(uint8_t *response, uint8_t* addr_reg_, uint8_t dev_add_){
		HAL_I2C_Master_Transmit(&bx_i2c_, dev_add_, addr_reg_, 1, HAL_MAX_DELAY); //send a transmit to the register interested in

		uint8_t response_size_ = sizeof(&response)/sizeof(uint8_t); //derive response size from the expected size of the response
																	// TODO: Make this memory safe
		HAL_I2C_Master_Receive(&bx_i2c_, dev_add_, response, response_size_, HAL_MAX_DELAY);
	}
}

