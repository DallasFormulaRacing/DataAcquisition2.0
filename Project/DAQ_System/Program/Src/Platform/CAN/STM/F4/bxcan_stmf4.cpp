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

#include "bxcan_stmf4.hpp"

namespace platform {

BxCanStmF4::BxCanStmF4(CAN_HandleTypeDef &hcan, CAN_FilterTypeDef &filter, const std::vector<uint32_t> &can_id_list)
  : bx_can_(hcan) {

	for (uint8_t i = 0; i < can_id_list.size(); i++) {
		ConfigureFilter(filter, i, can_id_list.at(i));
	}

	HAL_CAN_Start(&bx_can_);
}

void BxCanStmF4::ConfigureFilter(CAN_FilterTypeDef &filter, uint8_t filter_bank_num, uint32_t can_id) {
	filter.FilterIdHigh = can_id >> 13 ;
	filter.FilterIdLow = ((can_id & 0x1FFF) <<3) | CAN_ID_EXT | CAN_RTR_DATA ;
	filter.FilterMaskIdHigh = 0xFFFF;
	filter.FilterMaskIdLow = 0xFFFF;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterBank = filter_bank_num;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	filter.FilterActivation = CAN_FILTER_ENABLE;
	filter.SlaveStartFilterBank = 17;

	HAL_CAN_ConfigFilter(&bx_can_, &filter);
}


BxCanStmF4::~BxCanStmF4() {
	HAL_CAN_Stop(&bx_can_);
}

void BxCanStmF4::EnableInterruptMode() {
	// TODO: Make this configurable
	HAL_CAN_ActivateNotification(&bx_can_, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void BxCanStmF4::DisableInterruptMode() {
	// TODO: Make this configurable
	HAL_CAN_DeactivateNotification(&bx_can_, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void BxCanStmF4::ClearMessageArrivedFlag() { message_arrived_ = false; }

bool BxCanStmF4::MessageArrivedFlag() { return message_arrived_; }

uint32_t BxCanStmF4::LatestCanId() { return can_id_; }

void BxCanStmF4::ReceiveCallback() {
	if (HAL_CAN_GetRxMessage(&bx_can_, CAN_RX_FIFO0, &rx_message_header_, rx_buffer_) == HAL_OK) {
		message_arrived_ = true;
		can_id_ = rx_message_header_.ExtId;
	}
}

void BxCanStmF4::Receive(uint8_t rx_buffer[kMaxBytes]) {
	for (int i = 0; i < kMaxBytes; i++) {
		rx_buffer[i] = rx_buffer_[i];
	}
}

void BxCanStmF4::Transmit(uint8_t tx_buffer) { }



}
