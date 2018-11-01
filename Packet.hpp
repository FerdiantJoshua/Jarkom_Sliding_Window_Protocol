#pragma once

#include "Constants.hpp"
#include <stdlib.h>
#include <cstdint>

using namespace std;

class Packet {
	private:
		uint8_t soh = SOH;
		uint32_t seqNum = 0;
		uint32_t dataLength = 0;
		uint8_t data[MAX_DATA_SIZE] = {0};
		uint8_t checksum = 0;

	public:
		Packet();
		Packet(uint32_t seqNum, uint32_t dataLength, uint8_t *data);
		Packet(const Packet& _packet);
		~Packet();
		Packet& operator=(const Packet& _packet);

		uint32_t getSeqNum() const;

		void print() const;
		// void Packet::printHex() const;
		bool validate() const;
};