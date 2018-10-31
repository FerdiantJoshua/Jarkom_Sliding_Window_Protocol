#include "Constants.hpp"

#include "Ack.hpp"
#include "Packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>

using namespace std;

int main (int argc, char const *argv[]) {
    struct sockaddr_in myAddress;
    struct sockaddr_in remoteAddress;
    socklen_t addrlen = sizeof(remoteAddress);
    int recvlen;
    int fd = 0;

    Packet buffer[BUFFER_SIZE];

    //Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    //Bind socket to any IP and a port
    memset((char *) &myAddress, '0', sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY = 0.0.0.0 = any available IP address
    myAddress.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0 ) {
        cout << "Bind error" << endl;
        return -1;
    }

    //Loop for listening with a new thread
    int buffIdx = 0;
    int ackSeq = 0;
    while (true) {
        cout << "Waiting on port : " << PORT << ". Current bufferIdx : " << buffIdx << endl;
        recvlen = recvfrom(fd, &buffer[buffIdx], sizeof(Packet), 0, (struct sockaddr*) &remoteAddress, &addrlen);
        cout << "received " << recvlen << " bytes from " << &remoteAddress <<" with packet-detail :" << endl;
        if (recvlen > 0 ) {
            // buffer[buffIdx].print();
            
            //Send ACK if package is valid, and NAK if not
            //Ack(1, x) if valid
            //Ack(0, x) if invalid
            if (buffer[buffIdx].validate()) {
                cout << "Package valid, sending ACK " << buffer[buffIdx].getSeqNum() << endl;
                if (sendto(fd, new Ack(1, buffer[buffIdx].getSeqNum()), sizeof(Ack), 0, (struct sockaddr *) &remoteAddress, addrlen) < 0) {
                    cout << "Send ACK failed" << endl;
                }
            } else {
                cout << "Package invalid, sending NAK " << buffer[buffIdx].getSeqNum() << endl;
                if (sendto(fd, new Ack(0, buffer[buffIdx].getSeqNum()), sizeof(Ack), 0, (struct sockaddr *) &remoteAddress, addrlen) < 0) {
                    cout << "Send NAK failed" << endl;
                }
            }

            ackSeq++;
            buffIdx++;
        }

    }

    return 0;
}

// g++ recvfile.cpp Packet.cpp Ack.cpp -o receiver