#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#pragma region defines

#define TCP 0

#pragma endregion

int port = 9002;
char IPAddress[] = "127.0.0.1";

void Error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv){
    
    //Create socket
    int netSocket = socket(AF_INET, SOCK_STREAM, TCP);

    //Address structure for socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    //{TODO} update this!
    address.sin_addr.s_addr = INADDR_ANY; //Target IP address  

    //Connect
    int connStat = connect(netSocket, (struct sockaddr*) &address, sizeof(address));

    if(connStat == -1)
        printf("Connection failed %d\n", connStat);
    else
        printf("Connected!\n");

    //Recieve data from the server
    char serverResponse[2048];
    recv(netSocket, &serverResponse, sizeof(serverResponse), 0);

    //Print recieved data
    printf("Response: %s\n", serverResponse);

    //Close socket
    close(netSocket);

    return 0;
}
