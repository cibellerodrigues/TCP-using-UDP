#include "Practical.h"
#include "clientUtils.h"
#include "packets.h"

int main(int argc, char *argv[]){
     if(argc != 4)
        DieWithUserMessage("Parameter(s)", "<HOSTNAME-OR-IP> <PORT> <FILENAME>");
    
    char *hostname = argv[1];
    char *port = argv[2];
    char *filename = argv[3];

    int SOCK_UDP;
    struct addrinfo *servAddrPtr;
    Pacote *pacote = malloc(sizeof(Pacote));

    if(atoi(port) < 0 || atoi(port) > 65535)
        DieWithSystemMessage("porta invalida");

    UDPSocketClient(port, hostname, &SOCK_UDP, &servAddrPtr);

    Handshake(&SOCK_UDP, servAddrPtr, &pacote);
       

    // switch (state){
    //     case SYN_SEND:
    //         state = sendUDP(filename, &SOCK_UDP, servAddrPtr, &pacote, SYN);   
    //         break;
    //     case CONNECTING:
    //         state = recvUDP(&SOCK_UDP, servAddrPtr, &pacote, SYN_ACK); 
    //         break;
    //     case ESTABLISHED:                      
    //         state = sendUDP(filename, &SOCK_UDP, servAddrPtr, &pacote, ACK);  
    //         break;
    //     case CHECK:
    //         state = recvUDP(&SOCK_UDP, servAddrPtr, &pacote, ACK);      
    //         //exit(1); 
    //     default:
    //         break;
    // }

    //sendUDP(filename, &SOCK_UDP, servAddrPtr);
}