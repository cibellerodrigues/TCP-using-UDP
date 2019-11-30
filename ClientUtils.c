#include "clientUtils.h"


void Handshake(const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote){
    send_msg(sockPtr, servAddr, pacote, SYN);
    recv_msg(sockPtr, servAddr, pacote);
    send_msg(sockPtr, servAddr, pacote, ACK);
}

void recv_msg(const int *sockPtr, struct addrinfo *servAddr, Pacote **pkt){
    Pacote* pacote = malloc(sizeof(Pacote)); 
    
    socklen_t servAddrLen = sizeof(*servAddr);

    char buffer[20] = "\0";

    ssize_t numBytesRcvd = recvfrom(*sockPtr, buffer, sizeof(Pacote), 0,
        (struct sockaddr *) servAddr, &servAddrLen);
    if (numBytesRcvd < 0){
        DieWithSystemMessage("recvfrom() falhou");
    }

    Decode(buffer, numBytesRcvd, pacote, NONE);
    
    fprintf(stdout, "[RECV] SEQ = %d, ACK = %d, ID = %d, %d", pacote->sequenceNumber, 
    pacote->ACKNumber, pacote->connID, pacote->flags);

    **pkt = *pacote;
}

void send_msg(const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote, Flags flag){
    Pacote *pkt;
    
    switch (flag){
        case SYN:
            pkt = constroiPacote(INIT_SEQ_CLIENT, 0, 0, SYN, NULL);     
        break;
        case ACK:
            pkt = constroiPacote(INIT_SEQ_CLIENT + 1, (*pacote)->sequenceNumber + 1, (*pacote)->connID, 
            ACK, NULL);
        break;
        case FIN:
            pkt = constroiPacote((*pacote)->ACKNumber, 0, (*pacote)->connID, 
            FIN, NULL);
        break;
        default:
        break;
    }

    uint8_t outbuf[sizeof(Pacote)];

    size_t reqSize = Encode(pkt, outbuf, sizeof(Pacote), 0, NONE);

    ssize_t numBytes = sendto(*sockPtr, outbuf, reqSize, 0,
      servAddr->ai_addr, servAddr->ai_addrlen);
    
    if (numBytes < 0)
        DieWithSystemMessage("sendto() falhou");
    else if (numBytes != reqSize)
         DieWithUserMessage("sendto() error", "enviou número inexperado de bytes");

    **pacote = *pkt;
}

void UDPSocketClient(char *port, char *hostname, int* const sockPtr, 
 struct addrinfo **servAddrPtr){

    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family = AF_UNSPEC;
    addrCriteria.ai_socktype = SOCK_DGRAM;
    addrCriteria.ai_protocol = IPPROTO_UDP;

    struct addrinfo *servAddr;
    int rtn = getaddrinfo(hostname, port, &addrCriteria, &servAddr);
    if(rtn != 0)
        DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtn));

    int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
      servAddr->ai_protocol);
    if (sock < 0)
        DieWithSystemMessage("socket() falhou");

    *sockPtr = sock;
    *servAddrPtr = servAddr;
    
}


void sendUDP(char *filename, const int *sockPtr, struct addrinfo *servAddr, Pacote **pacote, Flags flag){
    
    Pacote *pkt;
    char buffer[MAXSTRINGLENGTH] = "\0";
    size_t bufferLen;  
    FILE *file;
    file = fopen(filename, "r");

    machineStates estado = NONE;

    switch (flag)
    {
    case SYN:
        bufferLen = strlen(buffer);
        pkt = constroiPacote(INIT_SEQ_CLIENT, 0, 0, SYN, constroiPayload(buffer, bufferLen));
        estado = NONE;     
        break;

    case ACK:
        //fgets(buffer, MAXSTRINGLENGTH, file);
        bufferLen = strlen(buffer);
        pkt = constroiPacote((*pacote)->ACKNumber, (*pacote)->sequenceNumber + 1, (*pacote)->connID, 
        ACK, constroiPayload(buffer, bufferLen));
        estado = NONE;
        break;
    default:
        break;
    }

    uint8_t outbuf[BUFSIZE];

    // fprintf(stdout, "SEQ = %d, ACK = %d, ID = %d, %d", pkt->sequenceNumber, 
    // pkt->ACKNumber, pkt->connID, pkt->flags);

    size_t reqSize = Encode(pkt, outbuf, BUFSIZE, bufferLen, estado);

    ssize_t numBytes = sendto(*sockPtr, outbuf, reqSize, 0,
      servAddr->ai_addr, servAddr->ai_addrlen);
    
    if (numBytes < 0)
        DieWithSystemMessage("sendto() falhou");
    else if (numBytes != reqSize)
         DieWithUserMessage("sendto() error", "enviou número inexperado de bytes");

    **pacote = *pkt;

    //return estado;
}


void recvUDP(const int *sockPtr, struct addrinfo *servAddr, Pacote **pkt,
 Flags flag){
    
    Pacote* pacote = malloc(sizeof(Pacote)); 
    
    socklen_t servAddrLen = sizeof(*servAddr);

    machineStates state = NONE;
    
    char buffer[BUFSIZE] = "\0";

    ssize_t numBytesRcvd = recvfrom(*sockPtr, buffer, BUFSIZE, 0,
        (struct sockaddr *) servAddr, &servAddrLen);
    if (numBytesRcvd < 0){
        DieWithSystemMessage("recvfrom() falhou");
    }

    switch (flag)
    {
    case SYN_ACK:
        Decode(buffer,numBytesRcvd, pacote, state);              
        break;
    // case ACK:
    //     state = EXIT;         //não é permanente
    // default:
        break;
    }

    //Decode(buffer,numBytesRcvd, pacote, state);
    

    **pkt = *pacote;

}
