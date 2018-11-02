# Sliding Window Protocol
**02/11/2018**

1. [Ferdiant Joshua M](https://github.com/FerdiantJoshua) - 13516047
2. [Kevin Basuki](https://github.com/kevinbasuki) - 13516071
3. [Nicolaus Boby Ardhita](https://github.com/nicolausboby) 13516077

A  **sliding window protocol**  is a feature of packet-based  Data transmission protocols. Sliding window protocols are used where reliable in-order delivery of packets is required, such as in the  Data Link Layer OSI layer 2 as well as in the Transmission Control Protocol 
Conceptually, each portion of the transmission (packets in most data link layers, but bytes in TCP) is assigned a unique consecutive sequence number, and the receiver uses the numbers to place received packets in the correct order, discarding duplicate packets and identifying missing ones. The problem with this is that there is no limit on the size of the sequence number that can be required.

[Simulation](http://www.ccs-labs.org/teaching/rn/animations/gbn_sr/) by ccs-labs

[Packet loss tools ](https://github.com/alexei-led/pumba) by alexei-led

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
## Compile and Running

``` ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port> ```
<!-- -->
``` ./recvfile <filename> <windowsize> <buffersize> <port> ```

### example
> ./recvfile sample.txt 10 1024 8080
 <!-- -->
> ./sendfile sample.txt 10 1024 127.0.0.1 8080