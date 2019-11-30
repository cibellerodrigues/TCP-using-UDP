#include "packets.h"
#include "Practical.h"
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

Pacote* constroiPacote(uint32_t sequenceNumber, uint32_t ACKNumber, uint16_t connID, 
    uint16_t flags, void *payload) {
        Pacote* pac = malloc(sizeof(Pacote));

        pac->sequenceNumber = sequenceNumber;
        pac->ACKNumber = ACKNumber;
        pac->connID = connID;
        pac->flags = flags;
        pac->payload = payload;

        return pac;
}

Payload *constroiPayload(char *str, size_t tam_str){
    Payload* payload = malloc(sizeof(Payload));
    payload->msg = malloc(tam_str);

    memcpy(payload->msg, str, tam_str);

    return payload;
}

void alocaPayload(Pacote* pkt, size_t tam_str) {
    Payload* payload = malloc(sizeof(Payload));
    payload->msg = malloc(tam_str);
    pkt->payload = payload;
}


size_t Encode(Pacote *pkt, uint8_t *outBuf, size_t bufSize, size_t tam_str, machineStates state) {
    
    size_t size = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    
    if(bufSize < sizeof(Pacote)) {
        DieWithUserMessage("Buffer muito pequeno", "");
    }

    memset(outBuf, 0, bufSize);

    u32 = htonl(pkt->sequenceNumber);
    memcpy(outBuf + SEQ_NUMBER_SHIFT, &u32, UI32SIZE);
    size += UI32SIZE;
    
    u32 = htonl(pkt->ACKNumber);
    memcpy(outBuf + ACK_SHIFT, &u32, UI32SIZE);
    size += UI32SIZE;
    
    
    u16 = htons(pkt->connID);
    memcpy(outBuf + CONNID_SHIFT, &u16, UI16SIZE);
    size += UI16SIZE;
    
    u16 = htons(pkt->flags);
    memcpy(outBuf + FLAGS_SHIFT, &u16, UI16SIZE);
    size += UI16SIZE;

    switch (state)
    {
    case CHECK:
        size = encodeHello(pkt, outBuf, size, tam_str);
        break;
    default:
        break;
    }
    
    return size;
}


size_t encodeHello(Pacote *pkt, uint8_t *outbuf, size_t size, size_t tam_str) {
    printf("\ntam_str: %ld", tam_str);
    
    memcpy(outbuf+size, ((Payload*)pkt->payload)->msg, tam_str);
    
    size += tam_str;
    
    return size;
}



bool Decode(uint8_t *inBuf, size_t mSize, Pacote *pkt, machineStates state){

    uint16_t u16 = 0;
    uint32_t u32 = 0;
    
    if(mSize > (TAM_CABECALHO + TAM_PAYLOAD)) {
        return false;
    }
    
    memcpy(&u32, inBuf + SEQ_NUMBER_SHIFT, UI32SIZE);
    pkt->sequenceNumber = ntohl(u32);

    memcpy(&u32, inBuf + ACK_SHIFT, UI32SIZE);
    pkt->ACKNumber = ntohl(u32);
 
    memcpy(&u16, inBuf + CONNID_SHIFT, UI16SIZE);
    pkt->connID = ntohs(u16);
        
    memcpy(&u16, inBuf + FLAGS_SHIFT, UI16SIZE);
    pkt->flags = ntohs(u16);


    // SE STATE: ESTABLISHED NÃO DECODIFICA MENSAGEM
    // SE STATE: CHECK 

    switch (state)
    {
    case VERIF:
        decodeHello(pkt, inBuf, mSize - TAM_CABECALHO);    
        break;
    default:
        break;
    }
     
    return true;
}

void decodeHello(Pacote *pkt, uint8_t *inBuf, size_t tam_str) {

    alocaPayload(pkt, tam_str);
    memcpy(((Payload*)pkt->payload)->msg, inBuf + TAM_CABECALHO, tam_str);
}
