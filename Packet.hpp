#pragma once

#include <cstdint>

#define SOH (0x1)

class Packet {
	private:
		uint8_t soh = SOH;
		uint32_t seqNum = 0;
		uint32_t dataLength = 0;
		uint8_t data = 0;
		uint8_t checksum = 0;
		uint8_t cat = 0;

	public:
		Packet();
		Packet(uint32_t seqNum, uint32_t dataLength, uint8_t data);
		Packet(const Packet& _packet);
		~Packet();
		Packet& operator=(const Packet& _packet);
		void Print() const;
		bool validate() const;
};