/*R
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

class I2CStmF4 : public II2C{
public:
	I2CStmF4(I2C_HandleTypeDef &hi2c);

	virtual ~I2CStmF4();

	virtual void Receive(uint8_t* commands, uint8_t dev_add_, uint8_t maxBytes) override;

	virtual void Transmit(uint8_t* response, uint8_t dev_add_, uint8_t response_size) override;

private:
	I2C_HandleTypeDef& i2c_;
};

}
#endif /* SRC_PLATFORM_I2C_STM_I2CHALWRAPPER_STMF4_HPP_ */
