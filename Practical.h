#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

void DieWithUserMessage(const char *msg, const char *detail);

void DieWithSystemMessage(const char *msg);

enum{
  INIT_SEQ_SERVER = 4321,
  INIT_SEQ_CLIENT = 12345, 
};



enum sizeConstants {
  MAXSTRINGLENGTH = 512,
  BUFSIZE = 524,
};


typedef enum {
  NONE,
  HANDSHAKE,
  CONNECTING,
  CHECK,
  CONNECTION_ID,
  ESTABLISHED,
  SETUP_UDP,
  SYN_SEND, 
  VERIF,
  EXIT,
} machineStates;

#endif // PRACTICAL_H_
