#pragma once

#include "Constants.hpp"
#include <cstdint>

struct Ack {
	uint8_t ack;
	uint32_t nextSeqNum;
	uint8_t checksum;

	Ack();
	Ack(uint8_t ack, uint32_t nextSeqNum);
	bool validate() const;
};