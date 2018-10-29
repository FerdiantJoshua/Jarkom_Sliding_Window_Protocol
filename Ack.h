#pragma once

#include <cstdint>

struct Ack {
	uint8_t ack;
	uint32_t nextSeqNum;
	uint8_t checksum;

	Ack(uint8_t ack, uint32_t nextSeqNum);
	bool validate() const;
};