#include "Constants.hpp"
#include "Ack.hpp"
#include "Packet.hpp"

#include <ctime>
#include <thread>
#include <string.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Packet.hpp"

using namespace std;

int main(){

    //Open File
    //const char *fileName = argv[1];
    Packet buffer[BUFFER_SIZE];
    uint8_t dataRead[MAX_DATA_SIZE];
    uint32_t packetCounter = 0;
    uint32_t packetSize = 0;
    bool isAlreadyRead = false;

    FILE* file = fopen(/*fileName*/"test.txt", "r");
    if(file == NULL){
        cout << "Can't open file" << endl;
    } else {
        cout << "File opened" << endl;
    }

    while(!isAlreadyRead && (packetCounter < BUFFER_SIZE)){
            cout << "test" << endl;		//test


        packetSize = fread(dataRead, 1, MAX_DATA_SIZE, file);
            cout << packetSize << endl;	//test
            /*for (int i=0; i<bufferCounter; i++){
                buffer[i].print();
            }*/

        Packet *packet1 = new Packet(packetCounter, packetSize, dataRead);
        	//packet1->print();			//test
        buffer[packetCounter] = *packet1;
        	buffer[packetCounter].print();	//test


        packetCounter++;

        if(feof(file)){
        	isAlreadyRead = true;
        }

        /*if (packetSize == MAX_DATA_SIZE) {
            char temp[1];
            int next_buffer_size = fread(temp, 1, 1, file);
                cout << next_buffer_size << endl;
            if (next_buffer_size == 0) isAlreadyRead = true;
        } else if (packetSize < MAX_DATA_SIZE) {
            isAlreadyRead = true;
        }*/

        
    }

    


    /*if(fscanf(file, "%c", &c) == EOF){
        isAlreadyRead = true;
        cout << "File already read" << endl;
    }
    while(bufferCounter < BUFFER_SIZE && !EOF){
        uint8_t data[MAX_DATA_SIZE];
        uint32_t dataSizeCounter = 0;

        if(fscanf(file, "%c", &c) == EOF){
            isAlreadyRead = true;
            cout << "File already read" << endl;
        }
        while(!isAlreadyRead && (dataSizeCounter < MAX_DATA_SIZE)){
            data[dataSizeCounter] = c;
            cout << "test" << endl;
            cout << data[dataSizeCounter] << endl;

            if(fscanf(file, "%c", &c) == EOF){
                isAlreadyRead = true;
                cout << "File already read" << endl;
            }
        }

        Packet p = Packet(bufferCounter, dataSizeCounter, *data);
        buffer[bufferCounter] = p;
        bufferCounter++;
        
    }

    for(int i=1; i<=bufferCounter; i++){
        buffer[i].print();
    }
    */
}