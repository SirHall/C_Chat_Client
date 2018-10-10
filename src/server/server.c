#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#pragma region defines

#define TCP 0

#pragma endregion

#pragma region Function Identifiers

void *ListenToNewConnections(void *any);
void *RecieveMessages(void *any);
void RemoveConnectionPort(int connectionPort);
void SendMessageAll(char *message);
void SendMessageTarget(char *message, int socket);
void SendMessageAllExcept(char *message, int exceptionSocket);
int AddConnection(int connectionPort);
void RemoveConnectionPort(int connectionPort);

#pragma endregion

#pragma region Internal

int netSocket;
pthread_t newConnThread;
pthread_t recievedMsgThread;


#pragma endregion

#pragma region Mutexes

pthread_mutex_t printMutex;
pthread_mutex_t connectionsMutex;
pthread_mutex_t recievedMsgMutex;

#pragma endregion

int port = 9002;
int maxConnections = 10;
int currentConnections = 0;

int *connectionPorts;

char connectionMessage[256] = "Connection successful!\n";
char declineMessage[256] = "Connection declined, server full\n"; //{TODO} May want to setup proper error codes


char stdIn[2048];

int main(int argc, char *argv[]){
	connectionPorts = calloc(maxConnections, sizeof(int));
	for(int i = 0; i < maxConnections; i++)
		connectionPorts[i] = -1; //Initialise all default ports to -1

	netSocket = socket(AF_INET, SOCK_STREAM, TCP);
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	//---SETUP SERVER---//
	bind(netSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	listen(netSocket, 128);

	//--SETUP THREADS--//
	//Mutexes
	pthread_mutex_init(&printMutex, NULL);
	pthread_mutex_init(&connectionsMutex, NULL);
	pthread_mutex_init(&recievedMsgMutex, NULL);

	pthread_create(&newConnThread, NULL, ListenToNewConnections, NULL);
	pthread_create(&recievedMsgThread, NULL, RecieveMessages, NULL);

	pthread_join(newConnThread, NULL);
	pthread_join(recievedMsgThread, NULL);

	//{TODO} Destroy threads and mutexes
	return 0;
}

void *ListenToNewConnections(void *any){
	while(1){
		if(currentConnections < maxConnections){ //{TODO}This doesn't seem safe
			
			// if()
			// 	continue; 
			
			//This will block until a new connection can be accepted
			int clientSocket = accept(netSocket, NULL, NULL);
			
			pthread_mutex_lock(&connectionsMutex);
			pthread_mutex_lock(&printMutex);
			//Connection successful
			send(clientSocket, connectionMessage, sizeof(connectionMessage), 0);
			AddConnection(clientSocket);
			
			printf("%s: %d\n", "Got new connection", clientSocket);
			printf("Now has %d connection(s)\n", currentConnections);
			
			pthread_mutex_unlock(&printMutex);
			pthread_mutex_unlock(&connectionsMutex);
		}
	}	
}

int AddConnection(int connectionPort){
	int emptyConnectionIndex = -1;
	//Find first element in 'connectionPorts'
	for(int i = 0; i < maxConnections; i++){
		if(connectionPorts[i] == -1){
			currentConnections++;
			emptyConnectionIndex = i;
			connectionPorts[emptyConnectionIndex] = connectionPort;
			break;
		}
	}
	
	if(emptyConnectionIndex == -1) //{TODO} Could probably move printing outside this function
		printf("Could not add to 'connectionPort', no empty elements!\n");
	else
		printf("Added new connection port: %d\n", connectionPort);
	
	return emptyConnectionIndex != -1; //Returns whether could insert connectionPort
}

void RemoveConnectionPort(int connectionPort){
	for(int i = 0; i < maxConnections; i++){
		if(connectionPorts[i] == connectionPort){
			currentConnections--;
			close(connectionPort);
			connectionPorts[i] = -1;
			return; //{TODO} Make this a 'break'?
		}
	}
	printf("Could not find connectionPort '%d' in connectionPorts", connectionPort); //{TODO} Move error message out of the function?
}

//To be run asyncronously
void *RecieveMessages(void *any){
	//{TODO} Clear from memory
	char recievedMessage[2048] = ""; //{TODO} Move buffer size into member field	
	while(1){//{TODO} Replace loop
		if(currentConnections > 0){
			pthread_mutex_lock(&recievedMsgMutex);
			pthread_mutex_lock(&printMutex);
			for(int i = 0; i < maxConnections; i++){ //{TODO} Fix up!
				if(connectionPorts[i] == -1) 
						continue;
					// printf("%d\n", connectionPorts[i]);
					int msgSize = recv(
						connectionPorts[i], 
						recievedMessage, 
						sizeof(recievedMessage), 
						MSG_DONTWAIT
					);
				// pthread_mutex_lock(&connectionsMutex);
					if(msgSize <= 0)
						continue;
					fputs(recievedMessage, stdout);
					fflush(stdout);
					// printf("Hi there");
				// pthread_mutex_unlock(&connectionsMutex);
			}
			pthread_mutex_unlock(&recievedMsgMutex);
			pthread_mutex_unlock(&printMutex);
			// SendMessageAll(recievedMessage);
		}
	}
	return NULL;
}

void SendMessageTarget(char *message, int socket){
	send(socket, message, sizeof(message), 0);
}

void SendMessageAll(char *message){
	for(int i = 0; i < maxConnections; i++)
		if(connectionPorts[i] != -1) //If this connection exists
			SendMessageTarget(message, connectionPorts[i]);
}

void SendMessageAllExcept(char *message, int exceptionSocket){
	for(int i = 0; i < maxConnections; i++)
		if(connectionPorts[i] != -1 && connectionPorts[i] != exceptionSocket)
			SendMessageTarget(message, connectionPorts[i]);
}

void RunCommand(char **args){

}
