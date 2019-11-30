#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "packets.h"

void UDPSocket(char *port, int* const sockPtr);
void recvUDPServer(int* const socket, char *diretorio, Pacote **pkt, Flags flag);
void HandShake(int* const socket, Pacote **pkt);
void send_msg(int* const sock, Pacote **pacote);
void recv_msg(int* const socket, Pacote **pkt);
void sendUDPServer(int* const sock, Pacote **pacote, Flags flag);
// int recvSYN(int sock, Pacote *pkt);
// int sendSYN_ACK(int sock, Pacote *pacote);