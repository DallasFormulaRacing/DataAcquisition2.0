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

namespace platform{

class II2C{
public:
	static constexpr uint8_t kMaxBytes = 2;

	virtual ~II2C() = default;

	virtual void Start() = 0;

	virtual void Master_Recieve(uint8_t*) = 0;

	virtual void Master_Transmit(uint8_t*) = 0;

	virtual void SetResponse(uint8_t) = 0;
};

}


#endif /* SRC_PLATFORM_I2C_INTERFACES_II2C_HPP_ */
