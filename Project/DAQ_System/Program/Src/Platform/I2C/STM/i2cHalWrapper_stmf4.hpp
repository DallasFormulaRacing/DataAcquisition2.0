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

class i2cHalWrapperStmf4 : public II2C{
public:
	i2cHalWrapperStmf4(I2C_HandleTypeDef &hi2c);

	virtual ~i2cHalWrapperStmf4();

	virtual void Master_Recieve(uint8_t*) override;

	virtual void Master_Transmit(uint8_t*) override;

	virtual void SetResponse(uint8_t) override;

private:
	I2C_HandleTypeDef bx_i2c_;
	uint8_t slave_add_;
	uint8_t response_size_;
};

}
#endif /* SRC_PLATFORM_I2C_STM_I2CHALWRAPPER_STMF4_HPP_ */
