#ifndef PACKET_H
#define PACKET_H

#include <cstdint>

class Packet {
	private:
		uint8_t soh;
		uint32_t seqNum;
		uint32_t dataLength;
		//uint8_t data;
		uint8_t checksum;

	public:
		bool compare() const;
};

#endif