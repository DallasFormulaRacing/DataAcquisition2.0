/*
 * i2cHalWrapper_stmf4.cpp
 *
 *  Created on: Mar 4, 2024
 *      Author: Nico
 */
#include "i2cHalWrapper_stmf4.hpp"

namespace platform {

i2cHalWrapperStmf4::i2cHalWrapperStmf4(I2c_HandleTypeDef &hi2c){

	void Master_Transmit(uint8_t *commands){
		HAL_I2C_Master_Transmit(&bx_i2c_,slave_add_, commands, kMaxBytes);
		}
	void Master_Recieve(uint8_t *commands){

		}
	}
}

