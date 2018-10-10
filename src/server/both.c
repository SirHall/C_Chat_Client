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
		if(currentConnections < maxConnections){
			listen(netSocket, maxConnections);
			
			int clientSocket = accept(netSocket, NULL, NULL);
			
			pthread_mutex_lock(&printMutex);
			printf("%s: %d\n", "Got new connection", clientSocket); //{TODO} Not threadsafe!!!
			printf("Now has %d connections\n", currentConnections);
	
			if(currentConnections < maxConnections){
				//Connection succesful
				send(clientSocket, connectionMessage, sizeof(connectionMessage), 0);
				pthread_mutex_lock(&connectionsMutex);
				AddConnection(clientSocket); //{TODO} Not threadsafe!!!
				pthread_mutex_unlock(&connectionsMutex);
			}else{
				//Connection declined
				// send(clientSocket, declineMessage, sizeof(declineMessage), 0);
			}
			pthread_mutex_unlock(&printMutex);
		}
	}	
}

int AddConnection(int connectionPort){
	int emptyConnectionIndex = -1;
	
	//Find first element in 'connectionPorts'
	for(int i = 0; i < maxConnections; i++){
		if(connectionPorts[i] == -1){
			emptyConnectionIndex = i;
			break;
		}
	}
	
	if(emptyConnectionIndex == -1){ //{TODO} Could probably move printing outside this function
		//All connection slots have a port assigned
		printf("Could not add to 'connectionPort', no empty elements!\n");
	}else{
		printf("Added new connection port: %d\n", connectionPort);
		connectionPorts[emptyConnectionIndex] = connectionPort;
	}
	return emptyConnectionIndex != -1; //Returns whether could insert connectionPort
}

void RemoveConnectionPort(int connectionPort){
	for(int i = 0; i < maxConnections; i++){
		if(connectionPorts[i] == connectionPort){
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
		// pthread_mutex_lock(&recievedMsgMutex);
		pthread_mutex_lock(&printMutex);
		pthread_mutex_lock(&connectionsMutex);
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
			if(msgSize <= 0)
				continue;
			// printf("%s\n", recievedMessage);
			fputs(recievedMessage, stdout);
			fflush(stdout);
			// printf("Hi there");
		}
		// pthread_mutex_unlock(&recievedMsgMutex);
		pthread_mutex_unlock(&printMutex);
		pthread_mutex_unlock(&connectionsMutex);
		// SendMessageAll(recievedMessage);
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
