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

#include "../../Interfaces/ican.hpp"

#include "can.h"

#include <vector>

namespace platform {

class BxCanStmF4 : public ICan {
public:
	BxCanStmF4(CAN_HandleTypeDef &hcan, CAN_FilterTypeDef &filter, const std::vector<uint32_t> &can_id_list);

	virtual ~BxCanStmF4();

	virtual void EnableInterruptMode() override;

	virtual void DisableInterruptMode() override;

	virtual bool MessageArrivedFlag() override;

	virtual void ClearMessageArrivedFlag() override;

	virtual uint32_t LatestCanId() override;

	virtual void ReceiveCallback() override;

	virtual void Receive(uint8_t rx_buffer[kMaxBytes]) override;

	virtual void Transmit(uint8_t tx_buffer) override;

private:
	void ConfigureFilter(CAN_FilterTypeDef &filter, uint8_t filter_bank_num, uint32_t can_id);

	CAN_RxHeaderTypeDef rx_message_header_;

	CAN_HandleTypeDef& bx_can_;


	uint8_t rx_buffer_[kMaxBytes] = { 0 };

	bool message_arrived_ = false;
	uint32_t can_id_;
};

}

#endif // STM_F4_BXCAN_STMF4_H
