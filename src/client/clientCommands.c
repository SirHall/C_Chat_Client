#include "clientCommands.h"
#include "clientNetworking.h"
#include <string.h>
#include <arpa/inet.h>

void RunClientCommand(const char* input){
    //input[0] == ':' is expected to be true
    if(input[0] != ':')
        return;
    char* pos = 1;


    if(strncmp(input[1], CLNCMD_Connect, sizeof(CLNCMD_Connect))){
        //Connect command
        pos += sizeof(CLNCMD_Connect) + 1;
        in_addr_t address;
        int port; 
        char tmpstr[20];
        
        strncpy(tmpstr, pos, strchr(pos, ':') - 1);

        inet_pton(AF_INET, "192.0.2.33", &address);

        
        //{TODO} Setup to recieve address
        Connect(/*address, */port);

        // // now get it back and print it
        // inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
    }

}