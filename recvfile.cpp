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
    Packet bufferTemp;
    bool receivedPacket[BUFFER_SIZE] = {0};

    int curBuffIdx = 0;
    int lowestBuffIdx = 0;

    // Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    // Bind socket to any IP and a port
    memset((char *) &myAddress, '0', sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY = 0.0.0.0 = any available IP address
    myAddress.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0 ) {
        cout << "Bind error" << endl;
        return -1;
    }

    // Loop for listening with a new thread
    int i = 0;
    int packetSeqNum = 0;
    while (true) {
        cout << endl << "Waiting on port : " << PORT << ". Current bufferIdx : " << curBuffIdx << endl;
        recvlen = recvfrom(fd, &bufferTemp, sizeof(Packet), 0, (struct sockaddr*) &remoteAddress, &addrlen);
        cout << "received " << recvlen << " bytes from " << &remoteAddress <<" with packet-detail :" << endl;
        if (recvlen > 0 ) {
            if (DEBUGGING_MODE) { 
                bufferTemp.print();
            }
            packetSeqNum = bufferTemp.getSeqNum();
            
            /** Send ACK and store to buffer if package is valid, and send NAK if not
                   Valid = Ack(1, x)
                   Invalid = Ack(0, x)
            **/
            if (bufferTemp.validate()) {
                cout << ">   Package valid, sending ACK " << packetSeqNum << endl;
                if (sendto(fd, new Ack(1, packetSeqNum), sizeof(Ack), 0, (struct sockaddr *) &remoteAddress, addrlen) < 0) {
                    cout << "   >   Send ACK failed" << endl;
                }

                // Save to buffer if within receiver window
                if (packetSeqNum >= lowestBuffIdx && packetSeqNum < lowestBuffIdx + WINDOW_SIZE) {
                    buffer[curBuffIdx] = bufferTemp;
                    receivedPacket[curBuffIdx] = true;
                    curBuffIdx++;
                }

                // Slide window
                i = lowestBuffIdx;
                while (receivedPacket[i++]) {
                    lowestBuffIdx++;
                }

            } else {
                cout << ">   Package invalid, sending NAK " << packetSeqNum << endl;
                if (sendto(fd, new Ack(0, packetSeqNum), sizeof(Ack), 0, (struct sockaddr *) &remoteAddress, addrlen) < 0) {
                    cout << "   >   Send NAK failed" << endl;
                }
            }

        }

    }

    return 0;
}

// g++ recvfile.cpp Packet.cpp Ack.cpp -o receiver