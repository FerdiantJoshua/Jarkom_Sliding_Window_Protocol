#include "Packet.h"
#include <numeric>
using namespace std;

bool Packet::compare() const{
	uint8_t sum;
	sum = std::accumulate((uint8_t*) this, (uint8_t*) &checksum, 0);
	return checksum == sum;
}