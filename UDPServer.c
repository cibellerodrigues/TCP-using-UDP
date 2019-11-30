#include "Practical.h"
#include "serverUtils.h"
#include "packets.h"

int client_id = 0;

int main(int argc, char *argv[]){

    int UDP_SOCK;
    
    machineStates state = NONE;

    if(argc !=  3)
        DieWithUserMessage("Parameter(s)", "<PORT> <FILE-DIR>");
    
    
    char *port = argv[1];
    char *dir = argv[2];

    if(atoi(port) < 0 || atoi(port) > 65535)
        DieWithSystemMessage("porta invalida");
      
    Pacote *pacote = malloc(sizeof(Pacote));
    
    UDPSocket(port, &UDP_SOCK);
    HandShake(&UDP_SOCK, &pacote);
    
    
    

    // for(;;){
        
    // }
    // for(;;){
    //     switch (state){
    //         case SETUP_UDP:
    //             state = recvUDPServer(&UDP_SOCK,&pacote, SYN);
    //             client_id++;
    //             break;
    //         case CONNECTION_ID:
    //             pacote->connID = client_id;
    //             state = sendSYN_ACK(&UDP_SOCK, &pacote);
    //             break;
    //         case ESTABLISHED:
    //             //state = recvACK(UDP_SOCK);
    //             break;
    //         case CONNECTED:
    //             break;
    //         case FINISH:
    //             break;
    //     }
    // }
    //     recvUDPServer(UDP_SOCK, dir);
    
}