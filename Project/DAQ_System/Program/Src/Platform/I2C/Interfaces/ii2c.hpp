/*
 * ii2c.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Nico
 */

#ifndef II2C_H
#define II2C_H

#include <cstdint>
#include <vector>

namespace platform {

class II2C {
public:
	virtual ~II2C() = default;

	virtual void Receive(uint8_t *response, uint8_t dev_add_, uint8_t response_size_) = 0;

	virtual void Transmit(uint8_t *commands, uint8_t dev_add_, uint8_t maxBytes) = 0;

};

}

#endif /* SRC_PLATFORM_I2C_INTERFACES_II2C_HPP_ */
