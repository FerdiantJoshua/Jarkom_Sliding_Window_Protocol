#include "Packet.hpp"
#include <numeric>
#include <iostream>
using namespace std;

Packet::Packet() {
	this->soh = 0;
	this->seqNum = 0;
	this->dataLength = 0;
	this->data = 0;
	this->checksum = 0;	
}

Packet::Packet(uint32_t seqNum, uint32_t dataLength, uint8_t data) {
	uint8_t _checksum;
	this->soh = SOH;
	this->seqNum = seqNum;
	this->dataLength = dataLength;
	this->data = data;
	_checksum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0);
	// cout << int(_checksum) << endl;
	this->checksum = _checksum;
}

Packet::Packet(const Packet& _packet) {
	this->soh = _packet.soh;
	this->seqNum = _packet.seqNum;
	this->dataLength = _packet.dataLength;
	this->data = _packet.data;
	this->checksum = _packet.checksum;
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
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0);
	// cout << int(sum) << endl;
	return checksum == sum;
}

void Packet::Print() const {
	cout << "soh : " << int(this->soh) << endl;
	cout << "seqNum : " << int(this->seqNum) << endl;
	cout << "dataLength : " << int(this->dataLength) << endl;
	cout << "data : " << int(this->data) << endl;
	cout << "checksum : " << int(this->checksum) << endl << endl;
}