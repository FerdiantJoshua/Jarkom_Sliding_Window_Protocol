#include "Constants.hpp"

#include "Ack.hpp"
#include "Packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>

using namespace std;

mutex thread_mutex;

int main(int argc, char const *argv[]) {

    char *server = "127.0.0.1";
    struct sockaddr_in myAddress;
    struct sockaddr_in remoteAddress;
    socklen_t addrlen = sizeof(remoteAddress);
    int recvlen = 0;
    int fd = 0;

    Packet buffer[BUFFER_SIZE];
    Ack bufferAck;
    bool receivedACK[BUFFER_SIZE] = {0};
    

    for (uint8_t i=0; i<10; i++) {
        buffer[i] = Packet(i, 0, 0);
    }


    // Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    
    // Bind socket to any IP and a port
    memset((char *)&myAddress, '0', sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = 0.0.0.0 = any available IP address
    myAddress.sin_port = htons(PORT);

    
    // Convert host address to binary format, and store it in myAddress 
    if ((inet_aton(server, &myAddress.sin_addr)) < 0) {
        cout << "inet_aton error" << endl;
        return -1;
    }


    // Open and send file
    FILE *file = fopen(fname, 'rb');
    char buffer[BUFFER_SIZE];
    int buffer;

    
    // Loop for sending
    int lowestBuffIdx = 0;
    thread thread_sender(([](int fd, Packet* buffer, sockaddr_in myAddress, int *lowestBuffIdx) {

        int buffIdx = 0;
        clock_t thisTime = clock();
        clock_t lastTime = thisTime;

        while (true) {
            thisTime = clock();
            clock_t timer[WINDOW_SIZE]={0};
            for (int i = *lowestBuffIdx; i < *lowestBuffIdx + WINDOW_SIZE; i++){
                if (TIME_OUT < thisTime - timer[i]) {
                    cout << "timer i :" << timer[i] << " timer now :" << thisTime;
                    timer[i] = thisTime;
                    cout << endl << "Sender's lowest buff idx : " << *lowestBuffIdx << endl;
                    cout << "Packet " << *lowestBuffIdx + i << " timeout, resending packet : " << *lowestBuffIdx + i << endl;
                    if (sendto(fd, &buffer[*lowestBuffIdx + i], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                        cout << "Send packet failed" << endl;
                    }
                }
            }

            // Send packet 1/second
            if (((thisTime - lastTime) >= ONE_SECOND) && (buffIdx < *lowestBuffIdx + WINDOW_SIZE)) {
                cout << endl << "Sender's lowest buff idx : " << *lowestBuffIdx << endl;
                cout << "Sending packet " << buffIdx << endl;
                if (sendto(fd, &buffer[buffIdx], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                    cout << "Send packet failed" << endl;
                } else {
                    timer[*lowestBuffIdx + ] =
                }
                buffIdx++;
                lastTime = thisTime;
            }
        }
    }), fd, buffer, myAddress, &lowestBuffIdx);

    
    // Listen from receiver for ACK
    int i = 0;
    while (true) {
        cout << endl << "Receiver's lowest buff idx : " << lowestBuffIdx << endl;
        cout << "Waiting on port : " << PORT << endl;
        recvlen = recvfrom(fd, &bufferAck, sizeof(Ack), 0, (struct sockaddr*) &myAddress, &addrlen);
        cout << "received " << recvlen << " bytes from " << &remoteAddress <<" with packet-detail :" << endl;
        if (recvlen > 0 ) {
            // buffer[buffIdx].print();
            
            // Resend package for the corresponding NAK received
            // Ack(1, x) is normal ACK
            // Ack(0, x) is NAK
            if (bufferAck.validate()) {
                cout << ">   ACK " << bufferAck.nextSeqNum << " valid, checking ACK type " << endl;
                // Resend packet if received ACK is an NAK
                if (bufferAck.ack == 0) {
                    cout << "   >   NAK detected, resending package " << bufferAck.nextSeqNum << endl;
                    if (sendto(fd, &buffer[bufferAck.nextSeqNum], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                        cout << "      >   Send packet failed" << endl;
                    }
                // Else no problem
                } else {
                    cout << "   >   ACK detected, finish with package " << bufferAck.nextSeqNum << endl;
                    receivedACK[bufferAck.nextSeqNum] = true;
                    
                    // Slide window
                    i = lowestBuffIdx;
                    while (receivedACK[i++]) {
                        receivedACK[lowestBuffIdx] = 0;
                        lowestBuffIdx++;
                    }
                }
            }
        }
    }

    return 0;
}