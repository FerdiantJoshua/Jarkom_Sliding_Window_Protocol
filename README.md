# Sliding Window Protocol
**02/11/2018**

1. [Ferdiant Joshua M](https://github.com/FerdiantJoshua) - 13516047
2. [Kevin Basuki](https://github.com/kevinbasuki) - 13516071
3. [Nicolaus Boby Ardhita](https://github.com/nicolausboby) 13516077

A  **sliding window protocol**  is a feature of packet-based  Data transmission protocols. Sliding window protocols are used where reliable in-order delivery of packets is required, such as in the  Data Link Layer OSI layer 2 as well as in the Transmission Control Protocol 
Conceptually, each portion of the transmission (packets in most data link layers, but bytes in TCP) is assigned a unique consecutive sequence number, and the receiver uses the numbers to place received packets in the correct order, discarding duplicate packets and identifying missing ones. The problem with this is that there is no limit on the size of the sequence number that can be required.

[Simulation](http://www.ccs-labs.org/teaching/rn/animations/gbn_sr/) by ccs-labs

[Packet loss tools ](https://github.com/alexei-led/pumba) by alexei-led

In this program, sliding window will traverse on the buffered data, and send the packets according to window size. receiver will receive packets and respond by sending ACK or NAK. sender then validate and send next packet if ACK was received or resend the packet loss if NAK was received instead.

+ sendfile:
	1. create socket and bind socket
	2. read input file
	3. put to buffer
	4. send file
	5. listen to receiver
	6. slide window

+ recvfile:
	1. open file
	2. create UDP socket and bind
	3. listening thread for sender
	4. send respond
	5. write packet to file
	6. slide window
---

## Frame
SOH (0x1)| Sequence Number|Data Length|Data|Checksum
| ------ |----------------| ----------|--- | ---
|1 Byte | 4 Byte | 4 Byte | Max 1024 Byte |1 Byte|

## Ack
Ack | Next Sequence Number|Checksum
| ------ |----------------| ----------|
|1 Byte | 4 Byte | 1 Byte | 

---
## Makefile
``` make ```

## Run program

``` ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port> ```
<!-- -->
``` ./recvfile <filename> <windowsize> <buffersize> <port> ```

### example
> ./recvfile sample.txt 10 1024 8080
 <!-- -->
> ./sendfile sample.txt 10 1024 127.0.0.1 8080

### Pembagian tugas
Ferdiant Joshua M.	: sendfile, recvfile, Packet, ACK, UDP socket
<!-- -->
Kevin Basuki 		: Struktur ACK, Packet, readfile(sendfile), writefile(recvfile)
<!-- -->
Nicolaus Boby		: sendfile, recvfile, Packet, ACK, mutex
<!-- -->