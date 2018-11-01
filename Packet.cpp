#include "Packet.hpp"
#include <string.h>
#include <numeric>
#include <bitset>
#include <iostream>
using namespace std;

Packet::Packet() {
	this->soh = 0;
	this->seqNum = 0;
	this->dataLength = MAX_DATA_SIZE;
	this->data = (uint8_t *) malloc(this->dataLength * sizeof(uint8_t));
	memset(this->data, 0, this->dataLength);
	this->checksum = 0;	
	// this->print();
}

Packet::Packet(uint32_t seqNum, uint32_t dataLength, uint8_t *data) {
	uint8_t _checksum;
	this->soh = SOH;
	this->seqNum = seqNum;
	this->dataLength = dataLength;
	cout << "this is data address : " << data << endl;
	memcpy(this->data, data, sizeof(uint8_t) * dataLength);
	// cout << "i survive" << endl;
	_checksum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	// cout << int(_checksum) << endl;
	this->checksum = _checksum;
	this->print();
}

Packet::Packet(const Packet& _packet) {
	this->soh = _packet.soh;
	this->seqNum = _packet.seqNum;
	this->dataLength = _packet.dataLength;
	this->data = _packet.data;
	this->checksum = _packet.checksum;
	// this->print();
}

Packet::~Packet() {

}

Packet& Packet::operator=(const Packet& _packet) {
	this->soh = _packet.soh;
	this->seqNum = _packet.seqNum;
	this->dataLength = _packet.dataLength;
	this->data = _packet.data;
	this->checksum = _packet.checksum;
}

bool Packet::validate() const{
	uint8_t sum;
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0) ^ (0xFF);
	// cout << int(sum) << endl;
	return checksum == sum;
}

uint32_t Packet::getSeqNum() const {
	return this->seqNum;
}

void Packet::print() const {
	cout << "soh : " << bitset<8>(this->soh) << endl;
	cout << "seqNum : " << bitset<32>(this->seqNum) << endl;
	cout << "dataLength : " << bitset<32>(this->dataLength) << endl;
	cout << "data : " << bitset<64>(*this->data) << endl;
	cout << "checksum : " << bitset<8>(this->checksum) << endl << endl;
}