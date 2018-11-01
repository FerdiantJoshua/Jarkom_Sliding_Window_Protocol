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
	memset(this->data, 0, this->dataLength * sizeof(uint8_t));
	this->checksum = 0;	
	
	// cout << "empty constructor, size of data : " << sizeof(this->data) << endl;
	// this->print();
}

Packet::Packet(uint32_t seqNum, uint32_t dataLength, uint8_t *data) {
	uint8_t _checksum;
	this->soh = SOH;
	this->seqNum = seqNum;
	this->dataLength = dataLength;
	// cout << "this is data address : " << data << endl;
	memcpy(this->data, data, dataLength * sizeof(uint8_t));
	/** This function (accumulate)) count soh + seqNum + dataLength + every values in data.
		   I separate the soh from accumulate because the C++ set the structure of the class in shape of :
		   | (1B 3B) 								   | (4B)	   | (4B)		   | (1024B) 	| (1B 3B)									   |
		   | 1B SOH + 3B unused-randomed value bytes | 4B seqNum | 4B dataLength | 1024B data | 1B checksum + 3B unused-randomed-value bytes |
	**/
	_checksum = (this->soh + std::accumulate((uint8_t*) &this->seqNum, (uint8_t*) &checksum, 0)) ^ (0xFF);
	this->checksum = _checksum;

	cout << "parametered constructor, size of data : " << sizeof(this->data) << endl;
	this->print();
}

Packet::Packet(const Packet& _packet) {
	this->soh = _packet.soh;
	this->seqNum = _packet.seqNum;
	this->dataLength = _packet.dataLength;
	memcpy(this->data, _packet.data, dataLength * sizeof(uint8_t));
	this->checksum = _packet.checksum;
	
	cout << "copy constructor, size of data : " << sizeof(this->data) << endl;
	this->print();
}

Packet::~Packet() {

}

Packet& Packet::operator=(const Packet& _packet) {
	this->soh = _packet.soh;
	this->seqNum = _packet.seqNum;
	this->dataLength = _packet.dataLength;
	memcpy(this->data, _packet.data, dataLength * sizeof(uint8_t));
	this->checksum = _packet.checksum;
	
	cout << "operator=, size of data : " << sizeof(this->data) << endl;
	this->print();
}

bool Packet::validate() const{
	uint8_t sum;
	sum = (this->soh + std::accumulate((uint8_t*) &this->seqNum, (uint8_t*) &checksum, 0)) ^ (0xFF);
	// cout << ">    sum : " << bitset<8>(sum) << endl;
	return checksum == sum;
}

uint32_t Packet::getSeqNum() const {
	return this->seqNum;
}

void Packet::print() const {
	cout << "soh : " << bitset<8>(this->soh) << endl;
	cout << "seqNum : " << bitset<32>(this->seqNum) << endl;
	cout << "dataLength : " << bitset<32>(this->dataLength) << endl;
	cout << "data : " << bitset<8 * MAX_DATA_SIZE>(this->data) << endl;
	cout << "checksum : " << bitset<8>(this->checksum) << endl;
	cout << "valid : " << this->validate() << endl << endl;
}