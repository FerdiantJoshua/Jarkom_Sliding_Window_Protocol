#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

int main()  {


	for (auto j = 0; j< 8; j++)
		sum += *j;

	sum = (sum & 0xFF) + (sum >> 8);
	sum = (sum & 0xFF) + (sum >> 8);

	cout << sum
	return 0;
}