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
    if (argc < 6) {
        cerr << "input parameter invalid!" << endl;
        return -1;
    }
    const char* fileName = argv[1];
    const int windowSize = stoi(argv[2]);
    const int bufferSize = stoi(argv[3]);
    const char *server = argv[4];
    const int port = stoi(argv[5]);

    // char *server = "127.0.0.1";
    struct sockaddr_in myAddress;
    struct sockaddr_in remoteAddress;
    socklen_t addrlen = sizeof(remoteAddress);
    int recvlen = 0;
    int fd = 0;

    Packet *buffer = (Packet*) malloc(sizeof(Packet) * bufferSize);
    Ack bufferTemp;
    bool receivedACK[bufferSize] = {0};
    
    int lowestBuffIdx = 0;

    /**==========READ FILE AND STORE TO BUFFER START==========**/
    uint8_t dataRead[MAX_DATA_SIZE];
    uint32_t packetCounter = 0;
    uint32_t packetSize = 0;
    bool isAlreadyRead = false;

    FILE* file = fopen(/*fileName*/fileName, "r");
    if(file == NULL){
        cout << "Can't open file" << endl;
    } else {
        cout << "File opened" << endl;
    }
    // Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    
    // Bind socket to any IP and a port
    memset((char *)&myAddress, '0', sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = 0.0.0.0 = any available IP address
    myAddress.sin_port = htons(port);

    
    // Convert host address to binary format, and store it in myAddress 
    if ((inet_aton(server, &myAddress.sin_addr)) < 0) {
        cout << "inet_aton error" << endl;
        return -1;
    }
    while(!isAlreadyRead) {
        packetCounter = 0;
        while(packetCounter < bufferSize){
            thread_mutex.lock();
            packetSize = fread(dataRead, 1, MAX_DATA_SIZE, file);
                //cout << packetSize << endl;	//test
                /*for (int i=0; i<bufferCounter; i++){
                    buffer[i].print();
                }*/

            Packet *packet1 = new Packet(packetCounter, packetSize, dataRead);
                //packet1->print();			//test
            buffer[packetCounter++] = *packet1;
                //buffer[packetCounter].print();	//test



            if(feof(file)){
                isAlreadyRead = true;
                buffer[packetCounter] = Packet(2,packetCounter);
                break;
            }
        }
        thread_mutex.unlock();
        /**==========READ FILE AND STORE TO BUFFER END==========**/


        /**==========DATA FOR TESTING START==========**/
        // uint8_t data[MAX_DATA_SIZE] = {0};

        // data[0] = 1;
        // data[1] = 255;
        // data[2] = 1;
        // data[3] = 1;
        // data[4] = 1;
        
        // // Testing and filling all packets
        // for (uint8_t i=0; i<10; i++) {
        //     buffer[i] = Packet(i, MAX_DATA_SIZE, data);
        // }
        /**==========DATA FOR TESTING END==========**/
      
        // Loop for sending
        thread thread_sender(([](int fd, Packet* buffer, sockaddr_in myAddress, int *lowestBuffIdx, int windowSize, int bufferSize) {
            thread_mutex.lock();
            int currBuffIdx = 0;
            clock_t thisTime = clock();
            clock_t lastTime = thisTime;
            clock_t timer[bufferSize]={thisTime};

            while (true) {
                thisTime = clock();

                // Don't send Packet with SOH = 0
                if (buffer[currBuffIdx].getSoh() != 0) {
                    for (int i = *lowestBuffIdx; i < *lowestBuffIdx + windowSize; i++){
                        if (TIME_OUT < thisTime - timer[i]) {
                            timer[i] = thisTime;
                            cout << endl << "Sender's lowest buff idx : " << *lowestBuffIdx << endl;
                            cout << "Packet " << i << " timeout, resending packet : " << i << endl;
                            if (sendto(fd, &buffer[i], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                                cout << "Send packet failed" << endl;
                            }
                        }
                    }

                    /** Send 1 packet/second
                     *  Only send buffer within window size
                     */
                    if (((thisTime - lastTime) >= ONE_SECOND) && (currBuffIdx < *lowestBuffIdx + windowSize)) {
                        cout << endl << "Sender's lowest buff idx : " << *lowestBuffIdx << endl;
                        cout << "Sending packet " << currBuffIdx << endl;
                        if (sendto(fd, &buffer[currBuffIdx], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                            cout << "Send packet failed" << endl;
                        } else {
                            timer[currBuffIdx] = thisTime;
                        }
                        currBuffIdx++;
                        lastTime = thisTime;
                    }
                }
            }
        }), fd, buffer, myAddress, &lowestBuffIdx, windowSize, bufferSize);
        thread_mutex.unlock();
    }
    fclose(file);
    // Listen from receiver for ACK
    int i = 0;
    int ackSeqNum = 0;
    while (true) {
        cout << endl << "Receiver's lowest buff idx : " << lowestBuffIdx << endl;
        cout << "Waiting on port : " << port << endl;
        recvlen = recvfrom(fd, &bufferTemp, sizeof(Ack), 0, (struct sockaddr*) &myAddress, &addrlen);
        cout << "received " << recvlen << " bytes from " << &remoteAddress <<" with packet-detail :" << endl;
        if (recvlen > 0 ) {            
            ackSeqNum = bufferTemp.nextSeqNum;
            
            /** Resend package for the corresponding NAK received
             *      Ack(1, x) is normal ACK
             *      Ack(0, x) is NAK
            **/
            if (bufferTemp.validate()) {
                cout << ">   ACK " << ackSeqNum << " valid, checking ACK type " << endl;
                // Resend packet if received ACK is an NAK
                if (bufferTemp.ack == 0) {
                    cout << "   >   NAK detected, resending package " << ackSeqNum << endl;
                    if (sendto(fd, &buffer[ackSeqNum], sizeof(Packet), 0, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
                        cout << "      >   Send packet failed" << endl;
                    }
                } else {  // Else no problem
                    cout << "   >   ACK detected, finish with package " << ackSeqNum << endl;
                    receivedACK[ackSeqNum] = true;
                    
                    // Slide window and update array of finished buffer
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

// g++ sendfile.cpp Packet.cpp Ack.cpp -o sender -lpthread