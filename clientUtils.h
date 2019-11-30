#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"
#include "packets.h"

void UDPSocketClient(char *port, char *hostname, int* const sockPtr, 
struct addrinfo **servAddrPtr);
void Handshake(const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote);
void recv_msg(const int *sockPtr, struct addrinfo *servAddr, Pacote **pkt);
void send_msg(const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote, Flags flag);
void sendUDP(char *filename, const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote, Flags flag);
void recvUDP(const int *sockPtr, struct addrinfo *servAddr, Pacote **pkt,
 Flags flag);