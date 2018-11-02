all: recvfile sendfile

recvfile: src/Ack.hpp src/Ack.cpp src/Packet.hpp src/Packet.cpp src/recvfile.cpp
	g++ src/recvfile.cpp src/Packet.cpp src/Ack.cpp -std=gnu++17 -o recvfile -lpthread

sendfile: src/Ack.hpp src/Ack.cpp src/Packet.hpp src/Packet.cpp src/sendfile.cpp
	g++ src/sendfile.cpp src/Packet.cpp src/Ack.cpp -std=gnu++17 -o sendfile -lpthread

clean: recvfile sendfile
	rm -f recvfile sendfile
