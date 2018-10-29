#include "Ack.h"
#include "Packet.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {
    Ack ack(0, 0);
    Packet packet(0, 0, 0, 0);

    cout << ack.validate() << endl;
    cout << packet.validate() << endl;
    
    return 0;
}