#include "Ack.h"
#include <numeric>
#include <iostream>
using namespace std;

Ack::Ack(uint8_t ack, uint32_t nextSeqNum) {
	uint8_t _checksum;
	this->ack = ack;
	this->nextSeqNum = nextSeqNum;
	_checksum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	cout << int(_checksum) << endl;
	this->checksum = _checksum;
}

bool Ack::validate() const{
	uint8_t sum;
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0)  ^ (0xFF);
	cout << int(sum) << endl;
	return checksum == sum;
}