#include "Constants.hpp"

#include "Ack.hpp"
#include "Packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>

using namespace std;

int main (int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in sender_address;
    socklen_t addrlen = sizeof(sender_address);
    int recvlen;
    int fd = 0;

    Packet *buffer = (Packet*) malloc(BUFFER_SIZE * sizeof(Packet));

    //Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    //Bind socket to any IP and a port
    memset((char *) &address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY = 0.0.0.0 = any available IP address
    address.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0 ) {
        cout << "Bind error" << endl;
        return -1;
    }

    //Loop for listening
    while (true) {
        cout << "Waiting on port : " << PORT << endl;
        recvlen = recvfrom(fd, &buffer[0], sizeof(Packet), 0, (struct sockaddr*) &address, &addrlen);
        cout << "received " << recvlen << " bytes with packet-detail :" << endl;
        if (recvlen > 0 ) {
            // buffer[recvlen] = NULL;
            buffer[0].Print();
        }
    }

    return 0;
}