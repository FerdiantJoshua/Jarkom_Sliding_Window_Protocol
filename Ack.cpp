#include "Ack.hpp"
#include <numeric>
#include <iostream>
using namespace std;

Ack::Ack() {
	this->ack = 0;
	this->nextSeqNum = 0;
	this->checksum = 0;
}

Ack::Ack(uint8_t ack, uint32_t nextSeqNum) {
	uint8_t _checksum;
	this->ack = ack;
	this->nextSeqNum = nextSeqNum;
	_checksum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	// cout << int(_checksum) << endl;
	this->checksum = _checksum;
}

bool Ack::validate() const{
	uint8_t sum;
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0)  ^ (0xFF);
	return checksum == sum;
}