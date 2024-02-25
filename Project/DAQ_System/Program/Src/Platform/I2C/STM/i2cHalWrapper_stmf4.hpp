/*
 * i2cHalWrapper_stmf4.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Nico
 */

#ifndef I2CHALWRAPPER_STMF4_HPP_
#define STM_I2CHALWRAPPER_STMF4_HPP_

#include "i2c.h"

#include "../../Interfaces/ii2c.hpp"

class i2cHalWrapperStmf4 : public II2C {
public:
	i2cHalWrapperStmf4(I2C_HandleTypeDef &hi2c);

	virtual ~i2cHalWrapperStmf4();

	virtual void Start() override;

	virtual void Recieve() override;

	virtual void Transmit() override;

};

#endif /* SRC_PLATFORM_I2C_STM_I2CHALWRAPPER_STMF4_HPP_ */
