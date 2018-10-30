#include "Constants.hpp"

#include "Ack.hpp"
#include "Packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char const *argv[]) {
    int fd = 0;
    char *server = "127.0.0.1";
    struct sockaddr_in server_address, address;

    Packet *buffer = (Packet*) malloc(BUFFER_SIZE * sizeof(Packet));
    
    //Testing and filling 5 first packets
    for (int i=0; i<5; i++) {
        buffer[i] = Packet(i, 0,0);
    }

    // buffer[0].Print();

    //Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    //Bind socket to any IP and a port
    memset((char *)&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = 0.0.0.0 = any available IP address
    server_address.sin_port = htons(PORT);

    //Convert host address to binary format, and store it in server_address 
    if ((inet_aton(server, &server_address.sin_addr)) < 0) {
        cout << "inet_aton error" << endl;
        return -1;
    }

    for (int i=0; i<5; i++) {
        cout << "Sending packet " << i << endl;
        if (sendto(fd, &buffer[i], sizeof(Packet), 0, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            cout << "Sendto failed" << endl;
            return -1;
        }
    }

    return 0;
}