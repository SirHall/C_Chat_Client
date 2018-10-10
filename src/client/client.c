#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include "ncurseEZ.h"
#include "messageBuffer.h"
#include "SendBuffer.h"

#pragma region defines

#define TCP 0
#define true 1
#define false 0

#pragma endregion

#pragma region progInternals

int netSocket;
int connStat;
struct sockaddr_in address;
char serverResponse[2048];

pthread_t readRecThread;

int typedKey;
char inVal[2];

#pragma endregion

#pragma region Function Identifiers

void Error(const char *msg);
void SetupNet();
void Connect();
void *RecieveFromServer();
void CloseConnection();
void SendMessage(const char *message);
void Loop();

#pragma endregion

#pragma region Mutexes

pthread_mutex_t IOMutex = PTHREAD_MUTEX_INITIALIZER;

#pragma endregion

int port = 9002;
int ip = 0;
// inet_pton(AF_INET, IPAddress, &ip);
// printf("Address: 0x%08x\n", ip);

char clientName[16] = "Unnamed";


void Error(const char *msg){
    perror(msg);
    exit(1);
}

//Client
int main(int argc, char *argv[]){

    Initialize();


    StartNCurses();

    SetupNet();

    Connect();

    //Initialize mutexes
    // pthread_mutex_init(&IOMutex, NULL);
    pthread_create(&readRecThread, NULL, RecieveFromServer, NULL);

    Loop();

    pthread_join(readRecThread, NULL);

    StopNCurses();

    return 0;
}

void SetupNet(){
    //Create socket
    netSocket = socket(AF_INET, SOCK_STREAM, TCP);

    //Address structure for socket
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    //{TODO} update this!
    address.sin_addr.s_addr = INADDR_ANY; //Target IP address  
}

void Connect(){
    //Connect
    connStat = connect(netSocket, (struct sockaddr*) &address, sizeof(address));

    if(connStat == -1)
        printw("Connection failed %d\n", connStat);
    else
        printw("Connected!\n");
}

//Must be run in thread
void *RecieveFromServer(void *any){
    //Recieve data from the server
    while(1){
        recv(netSocket, serverResponse, sizeof(serverResponse), 0);
        pthread_mutex_lock(&IOMutex);
        printw(serverResponse);
        pthread_mutex_unlock(&IOMutex);
    }
    return NULL;
}

void CloseConnection(){
    //Close socket
    close(netSocket);
}

void SendMessage(const char *message){
    //Send message to client

    printw(" -- Sent Message\n");
    send(netSocket, message, sizeof(message), 0);
}

//{TODO} Make this sendd the entire buffer instead of just 8 bytes at a time
void Loop(){
    for(;;){
        pthread_mutex_lock(&IOMutex);
		typedKey = getch();
		sprintf(inVal, "%c", typedKey);
		if(typedKey != -1){
            if(typedKey == 3) 
                break; //User pressed Ctrl+C
            else if(typedKey == KEY_ENTER || typedKey == 10){
                SendMessage(GetBuffer());
                Clear();}
            else if(typedKey == KEY_BACKSPACE)
                RemoveLastCharacter();
            else{
                AppendCharacter(*inVal);
                printw(inVal);
                // SendMessage(inVal);}//{TODO} Just here for testing! REMOVE
            }
        }
		pthread_mutex_unlock(&IOMutex);
        // else
		// 	printw("!");
        
	}
}