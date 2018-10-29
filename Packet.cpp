#include "Packet.h"
#include <numeric>
#include <iostream>
using namespace std;

Packet::Packet(uint8_t soh, uint32_t seqNum, uint32_t dataLength, uint8_t data) {
	uint8_t _checksum;
	this->soh = soh;
	this->seqNum = seqNum;
	this->dataLength = dataLength;
	this->data = data;
	_checksum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	cout << int(_checksum) << endl;
	this->checksum = _checksum;
}

bool Packet::validate() const{
	uint8_t sum;
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	cout << int(sum) << endl;
	return checksum == sum;
}