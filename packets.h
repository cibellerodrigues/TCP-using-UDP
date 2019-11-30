#ifndef PACKETS_H_
#define PACKETS_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include "Practical.h"

#define UI32SIZE 4
#define UI16SIZE 2

typedef enum {
    FIN = 1,
    SYN = 2,
    ACK = 4,
    SYN_ACK = 6,
    FIN_ACK = 5,
} Flags;

enum {
    SEQ_NUMBER_SHIFT = 0,
    ACK_SHIFT = 4,
    CONNID_SHIFT = 8,
    FLAGS_SHIFT = 10,
    TAM_CABECALHO = 12,
    TAM_PAYLOAD = 512,
};


struct payload{
    char *msg;
};

typedef struct payload Payload;


struct packet{
    uint32_t sequenceNumber;
    uint32_t ACKNumber;
    uint16_t connID;
    uint16_t flags;
    void *payload;
};

typedef struct packet Pacote;


Pacote* constroiPacote(uint32_t sequenceNumber, uint32_t ACKNumber, uint16_t connID, 
    uint16_t flags, void *payload);

void alocaPayload(Pacote* pkt, size_t tam_str);

Payload *constroiPayload(char *str, size_t tam_str);

size_t Encode(Pacote *pkt, uint8_t *outBuf, size_t bufSize, size_t tam_str, machineStates state);

size_t encodeHello(Pacote *pkt, uint8_t *outbuf, size_t size, size_t tam_str);

bool Decode(uint8_t *inBuf, size_t mSize, Pacote *pkt, machineStates state);

void decodeHello(Pacote *pkt, uint8_t *inBuf, size_t tam_str);



#endif