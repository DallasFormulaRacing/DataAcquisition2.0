/*
* CAN Abstract Interface
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef ICAN_H
#define ICAN_H

// Standard Libraries
#include <cstdint>

namespace platform {

class ICan {
public:
	static constexpr uint8_t kMaxBytes = 8;

	virtual ~ICan() = default;

	virtual void EnableInterruptMode() = 0;

	virtual void DisableInterruptMode() = 0;

	virtual bool MessageArrivedFlag() = 0;

	virtual void ClearMessageArrivedFlag() = 0;

	virtual uint32_t LatestCanId() = 0;

	virtual void ReceiveCallback() = 0;

	virtual void Receive(uint8_t rx_buffer[kMaxBytes]) = 0;

	virtual void Transmit(uint8_t tx_buffer) = 0;
};

}

#endif // ICAN_H
