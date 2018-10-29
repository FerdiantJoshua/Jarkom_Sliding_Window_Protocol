#pragma once

#include <cstdint>

struct Packet {
	uint8_t soh;
	uint32_t seqNum;
	uint32_t dataLength;
	uint8_t data;
	uint8_t checksum;

	Packet(uint8_t soh, uint32_t seqNum, uint32_t dataLength, uint8_t data);
	bool validate() const;
};