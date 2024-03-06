/*
 * i2cHalWrapper_stmf4.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Nico
 */

#ifndef I2CHALWRAPPER_STMF4_HPP_
#define STM_I2CHALWRAPPER_STMF4_HPP_

#include "../Interfaces/ii2c.hpp"
#include "i2c.h"

namespace platform{

class i2cHalWrapperStmf4 : public II2C{
public:
	i2cHalWrapperStmf4(I2C_HandleTypeDef &hi2c);

	virtual ~i2cHalWrapperStmf4();

	virtual void Start() override;

	virtual void Master_Recieve() override;

	virtual void Master_Transmit() override;

	virtual void Slave_Recieve() override; //that is, the STM32 transmiting and reciving as a slave, probably unnecessary

	virtual void Slave_Transmit() override;

private:

	I2C_HandleTypeDef bx_i2c_;
	uint8_t slave_add_;
};

}
#endif /* SRC_PLATFORM_I2C_STM_I2CHALWRAPPER_STMF4_HPP_ */
