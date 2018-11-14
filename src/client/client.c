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
#include "netConst.h"
#include "messageTypes.h"
#include "serverCommands.h"
#include "clientCommands.h"
#include "clientNetworking.h"

#pragma region defines

#define TCP 0
#define true 1
#define false 0

#pragma endregion

#pragma region progInternals

int netSocket;
int connStat;
struct sockaddr_in address;
char serverResponse[MSG_SIZE] = "";

pthread_t readRecThread;

int typedKey;
char inVal[2];

#pragma endregion

#pragma region Function Identifiers

void Error(const char *msg);
void SetupNet();
void *RecieveFromServer();
void CloseConnection();
void SendMessage(const char *message, int messageSize);
void Loop();
void InputEntered();

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

	//{TODO} Pull the sockaddr and target port from somewhere
	Connect(/*address,*/ port);

	//Initialize mutexes
	// pthread_mutex_init(&IOMutex, NULL);
	pthread_create(&readRecThread, NULL, RecieveFromServer, NULL);

	Loop();

	pthread_join(readRecThread, NULL);

	StopNCurses();

	return 0;
}

void SetupNet(){
	  
}

void Connect(/*struct sockaddr* targetAddress, */int targetPort){
	//Connect
	port = targetPort;
	
	//Create socket
	netSocket = socket(AF_INET, SOCK_STREAM, TCP);
	//Address structure for socket
	address.sin_family = AF_INET;
	address.sin_port = htons(targetPort);
	//{TODO} update this!
	address.sin_addr.s_addr = INADDR_ANY; //Target IP address

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
		for(int i = 0; i < sizeof(serverResponse); i++)
			serverResponse[i] = '\0';
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

void SendMessage(const char *message, int messageSize){
	//Send message to client
	// printw("Message size: %d\n", sizeof(message));
	send(netSocket, message, messageSize, 0);
}

//{TODO} Make this send the entire buffer instead of just 8 bytes at a time
void Loop(){
	for(;;){
		pthread_mutex_lock(&IOMutex);
		typedKey = getch();
		sprintf(inVal, "%c", typedKey);
		if(typedKey != -1){
			if(typedKey == 3) 
				break; //User pressed Ctrl+C
			else if(typedKey == KEY_ENTER || typedKey == 10){
				AppendCharacter('\n');
				printw("\n");
				InputEntered();
				Clear();}
			else if(typedKey == KEY_BACKSPACE)
				RemoveLastCharacter();
			else{
				AppendCharacter(*inVal);
				printw(inVal);
			}
		}
		pthread_mutex_unlock(&IOMutex);
	}
}

void InputEntered(){

	switch(GetBuffer()[0]){
		case CLNCMD_Start:
			//This is a local client command

			break;
		case SVRCMD_Start:
			//This is a server command
			
			break;
		default:
			//This is a text message
			SendMessage(GetBuffer(), GetBufferSize());
			Clear();
			break;
	}

	if(GetBuffer()[0] == CLNCMD_Start){
		//This is a client command
	}else if(GetBuffer()[0] == SVRCMD_Start){
		//This is a server command
	}

	SendMessage(GetBuffer(), strlen(GetBuffer()));
}