/*
 * i2cHalWrapper_stmf4.cpp
 *
 *  Created on: Mar 4, 2024
 *      Author: Nico
 */
#include "i2cHalWrapper_stmf4.hpp"

namespace platform {

	i2cHalWrapperStmf4::i2cHalWrapperStmf4(I2C_HandleTypeDef &hi2c)
			: bx_i2c_(hi2c) { }

	i2cHalWrapperStmf4::~i2cHalWrapperStmf4(){}

	void i2cHalWrapperStmf4::Master_Transmit(uint8_t *commands){
		HAL_I2C_Master_Transmit(&bx_i2c_,slave_add_, commands, kMaxBytes, 0xFFFFFFFF);
	}
	void i2cHalWrapperStmf4::Master_Recieve(uint8_t *response){
		HAL_I2C_Master_Receive(&bx_i2c_, slave_add_, response, response_size_, 0xFFFFFFFF);
	}
	void i2cHalWrapperStmf4::SetResponse(uint8_t size){
		response_size_ = size;
	}


}

