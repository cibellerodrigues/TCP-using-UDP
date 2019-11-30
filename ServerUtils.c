#include "serverUtils.h"
#include "Practical.h"
#include "packets.h"

void HandShake(int* const socket, Pacote **pkt){
    recv_msg(socket, pkt);
    (*pkt)->connID = 1;
    send_msg(socket, pkt);
    recv_msg(socket, pkt);
}

void send_msg(int* const sock, Pacote **pacote){
    Pacote *pkt;

    struct sockaddr_storage clntAddr;

    socklen_t clntAddrLen = sizeof(clntAddr);

    pkt = constroiPacote(INIT_SEQ_SERVER, (*pacote)->sequenceNumber+1, (*pacote)->connID, 
    SYN_ACK, constroiPayload(NULL, 0));

    uint8_t outbuf[sizeof(Pacote)];

    size_t reqSize = Encode(pkt, outbuf, sizeof(Pacote), 0, NONE);
   
    ssize_t numBytes = sendto(*sock, outbuf, reqSize, 0,
    (struct sockaddr *) &clntAddr, clntAddrLen);
    
    if (numBytes < 0)
        DieWithSystemMessage("sendto() falhou");
    else if (numBytes != reqSize)
         DieWithUserMessage("sendto() error", "enviou número inexperado de bytes");

    **pacote = *pkt;
}

void recv_msg(int* const socket, Pacote **pkt){
    Pacote *pacote = malloc(sizeof(Pacote));
    struct sockaddr_storage clntAddr;

    socklen_t clntAddrLen = sizeof(clntAddr);

    char buffer[sizeof(Pacote)] = "\0";

    ssize_t numBytesRcvd = recvfrom(*socket, buffer, sizeof(Pacote), 0,
    (struct sockaddr *) &clntAddr, &clntAddrLen);

    if(numBytesRcvd < 0)
        DieWithSystemMessage("recvfrom() falhou");

    Decode(buffer,numBytesRcvd, pacote, NONE);

    fprintf(stdout, "[RECV] SEQ = %d, ACK = %d, ID = %d, %d\n", pacote->sequenceNumber, 
    pacote->ACKNumber, pacote->connID, pacote->flags);

    **pkt = *pacote;
}


void UDPSocket(char *port, int* const sockPtr){

    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family = AF_UNSPEC;
    addrCriteria.ai_flags = AI_PASSIVE;
    addrCriteria.ai_socktype = SOCK_DGRAM;
    addrCriteria.ai_protocol = IPPROTO_UDP;

    struct addrinfo *servAddr;
    int rtn = getaddrinfo(NULL, port, &addrCriteria, &servAddr);
    if(rtn != 0)
        DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtn));

    int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
    servAddr->ai_protocol);
    if(sock < 0)
        DieWithSystemMessage("socket() falhou");

    int bd = bind(sock, servAddr->ai_addr, servAddr->ai_addrlen);
    if(bd < 0)
        DieWithSystemMessage("bind() falhou");

    *sockPtr = sock;
    
    freeaddrinfo(servAddr);
}

void sendUDPServer(int* const sock, Pacote **pacote, Flags flag){
    
    Pacote *pkt;

    struct sockaddr_storage clntAddr;

    socklen_t clntAddrLen = sizeof(clntAddr);

    pkt = constroiPacote(INIT_SEQ_SERVER, (*pacote)->sequenceNumber+1, (*pacote)->connID, 
    flag, constroiPayload(NULL, 0));

    uint8_t outbuf[BUFSIZE];

    size_t reqSize = Encode(pkt, outbuf, BUFSIZE, 0, NONE);
   
    ssize_t numBytes = sendto(*sock, outbuf, reqSize, 0,
    (struct sockaddr *) &clntAddr, clntAddrLen);
    
    if (numBytes < 0)
        DieWithSystemMessage("sendto() falhou");
    else if (numBytes != reqSize)
         DieWithUserMessage("sendto() error", "enviou número inexperado de bytes");

    **pacote = *pkt;

}

void recvUDPServer(int* const socket, char *diretorio, Pacote **pkt, Flags flag){
    Pacote *pacote = malloc(sizeof(Pacote));
   
    machineStates estado;

    struct sockaddr_storage clntAddr;

    socklen_t clntAddrLen = sizeof(clntAddr);

    char buffer[BUFSIZE] = "\0";

    ssize_t numBytesRcvd = recvfrom(*socket, buffer, MAXSTRINGLENGTH, 0,
    (struct sockaddr *) &clntAddr, &clntAddrLen);

    if(numBytesRcvd < 0)
        DieWithSystemMessage("recvfrom() falhou");

    switch (flag){
        case SYN:
            estado = CONNECTION_ID;
            Decode(buffer, numBytesRcvd, pacote, estado);
            break;
    case ACK:
        estado = NONE;
        Decode(buffer,numBytesRcvd, pacote, estado);
        // char *msg = (char *)calloc(MAXSTRINGLENGTH, sizeof(char));
        
        // msg = ((Payload *)pacote->payload)->msg;
        
        // ssize_t len = strlen(diretorio);
	    // char dir[len];
	    // strcpy (dir, diretorio);
        // strcat(dir, "/1.txt");

        // FILE *file;
        // file = fopen(dir ,"a");
        // if(fputs(msg, file) == EOF)
        //     DieWithSystemMessage("fputs() falhou");

        // fclose(file);
        // free(msg);

        default:
            break;
    }

    **pkt = *pacote;

    // ack = pacote->sequenceNumber + 1;

    // switch (state)
    // {
    // case NONE:
    //     psend = constroiPacote(INIT_SEQ_SERVER, ack, 0, SYN, constroiPayload(NULL, 0));
    //     break;
    
    // default:
    //     break;
    // }
        
    //fprintf( stdout, "num: %ld\n", numBytesRcvd);
        
}