#include "Constants.hpp"

#include "Ack.hpp"
#include "Packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <mutex>

using namespace std;
mutex listen_mutex;

int main (int argc, char const *argv[]) {
    if (argc < 5) {
        cerr << "input parameter invalid!" << endl;
        return -1;
    }
    const char* fileName = argv[1];
    const int windowSize = stoi(argv[2]);
    const int bufferSize = stoi(argv[3]);
    const int port = stoi(argv[4]);

    struct sockaddr_in myAddress;
    struct sockaddr_in remoteAddress;
    socklen_t addrlen = sizeof(remoteAddress);
    int recvlen;
    int fd = 0;

    Packet buffer[bufferSize];
    Packet bufferTemp;
    bool receivedPacket[bufferSize] = {0};

    int curBuffIdx = 0;
    int lowestBuffIdx = 0;

    //Open File
    FILE *file = fopen(fileName, "wb") ;

    // Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    // Bind socket to any IP and a port
    memset((char *) &myAddress, '0', sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY = 0.0.0.0 = any available IP address
    myAddress.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0 ) {
        cout << "Bind error" << endl;
        return -1;
    }

    // Loop for listening with a new thread
    bool finished_listen = false;
    while (!finished_listen){
        int i = 0;
        int packetSeqNum = 0;
        while (true) {
            listen_mutex.lock();
            cout << endl << "Waiting on port : " << port << ". Current bufferIdx : " << curBuffIdx << endl;
            recvlen = recvfrom(fd, &bufferTemp, sizeof(Packet), 0, (struct sockaddr*) &remoteAddress, &addrlen);
            cout << "received " << recvlen << " bytes from " << &remoteAddress <<" with packet-detail :" << endl;
            if (recvlen > 0 ) {
                if (DEBUGGING_MODE) { 
                    bufferTemp.print();
                }
                
                packetSeqNum = bufferTemp.getSeqNum();
                
                /** Send ACK and store to buffer if package is valid, and send NAK if not
                 *      Valid = Ack(1, x)
                 *      Invalid = Ack(0, x)
                **/
                if (bufferTemp.validate()) {
                    cout << ">   Package valid, sending ACK " << packetSeqNum << endl;
                    if (sendto(fd, new Ack(1, packetSeqNum), sizeof(Ack), 0, (struct sockaddr *) &remoteAddress, addrlen) < 0) {
                        cout << "   >   Send ACK failed" << endl;
                    }


                    if(bufferTemp.getSoh() == 2){
                        finished_listen = true;
                        break;
                    }

                    // Save to buffer if within receiver window
                    if (packetSeqNum >= lowestBuffIdx && packetSeqNum < lowestBuffIdx + windowSize) {
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
            listen_mutex.unlock();
        }

        listen_mutex.lock();
        for(int i=0; i<curBuffIdx; i++){
            if(i<(curBuffIdx-1)){
                fwrite(buffer[i].getData(), 1, bufferSize, file);
            }else{
                fwrite(buffer[i].getData(), 1, int(buffer[i].getDataLength()), file);
            }
        }
        listen_mutex.unlock();
    }
    fclose(file);
    return 0;
}

// g++ recvfile.cpp Packet.cpp Ack.cpp -std=gnu++17 -o receiver
// g++ recvfile.cpp Packet.cpp Ack.cpp -o receiver

// ./receiver result.txt 10 1024 8080