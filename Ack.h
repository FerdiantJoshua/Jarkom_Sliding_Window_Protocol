#ifndef ACK_H
#define ACK_H

#include <cstdint>

class Ack {
	private:
		uint8_t ack;
		uint32_t nextSeqNum;
		uint8_t checksum;

	public:
		bool compare() const;
};

#endif