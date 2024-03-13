/*
* BxCAN Peripheral
* Author:   William Lim
*           Cristian Cruz
*
* Email:    wll180000@utdallas.edu
*           cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef STM_F4_BXCAN_STMF4_H
#define STM_F4_BXCAN_STMF4_H

// Standard Libraries
#include <vector>

// ST HAL Dependencies
#include "can.h"

// DFR Custom Dependencies
#include "../../Interfaces/ican.hpp"


namespace platform {

class BxCanStmF4 : public ICan {
public:
	BxCanStmF4(CAN_HandleTypeDef &hcan);

	virtual ~BxCanStmF4();

	virtual void Start() override;

	virtual void EnableInterruptMode() override;

	virtual void DisableInterruptMode() override;

	virtual void Receive(uint8_t rx_buffer[kMaxBytes]) override;

	virtual void Transmit(uint8_t tx_buffer[kMaxBytes]) override;

	virtual bool MessageArrivedFlag() override;

	virtual void ClearMessageArrivedFlag() override;

	virtual uint32_t LatestCanId() override;

	enum class ReceiveInterruptMode : uint8_t {
		kFifo0MessagePending,
		kFifo0Full,
		Fifo0Overrun,

		kFifo1MessagePending,
		kFifo1Full,
		Fifo1Overrun,
	};

	void ConfigureFilter(uint32_t can_id, uint32_t filder_id_high, uint32_t filter_id_low);

	void ConfigureReceiveCallback(ReceiveInterruptMode mode);

	void ReceiveCallback();

private:

	uint8_t rx_buffer_[kMaxBytes] = { 0 };
	bool message_arrived_ = false;
	uint32_t can_id_ = 0;

	// ST's HAL library.
	CAN_HandleTypeDef& bx_can_; 			// BxCAN (Basic Extended) peripheral
	CAN_RxHeaderTypeDef rx_message_header_; // Receiving message info
	CAN_FilterTypeDef filter_;				// Filter bank configuration
	uint8_t filter_bank_num_;

	// Configurable options
	uint32_t rx_interrupt_mode_ = 0;
	uint8_t rx_fifo_ = 0;
};

}

#endif // STM_F4_BXCAN_STMF4_H
