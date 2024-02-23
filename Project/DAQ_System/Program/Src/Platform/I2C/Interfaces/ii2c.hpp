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

	static constexpr uint8_t MAG_ADD;

	static constexpr unit8_t ACC_ADD;

	virtual ~II2C() = default;

	virtual void Recieve() = 0;

	virtual void Transmit() = 0;
};

}


#endif /* SRC_PLATFORM_I2C_INTERFACES_II2C_HPP_ */
