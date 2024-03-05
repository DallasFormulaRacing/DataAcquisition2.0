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

	static constexpr uint8_t kMaxBytes = 2;

	virtual ~II2C() = default;

	virtual void Start() = 0;

	virtual void Master_Recieve() = 0;

	virtual void Master_Transmit() = 0;

	virtual void Slave_Recieve() = 0;

	virtual void Slave_Transmit() = 0;

};

}


#endif /* SRC_PLATFORM_I2C_INTERFACES_II2C_HPP_ */
