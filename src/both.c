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

#pragma region Function Identifiers

void *ListenToNewConnections();
void RemoveConnectionPort(int connectionPort);

#pragma endregion

int port = 9002;
int maxConnections = 1;
int currentConnections = 0;

int *connectionPorts;

char connectionMessage[256] = "Connection successful!\n";
char declineMessage[256] = "Connection declined, server ful\n"; //{TODO} May want to setup proper error codes

int netSocket;

int main(int argc, char *argv[]){
	connectionPorts = new int[maxConnections];
	//Initialise all default ports to -1
	for(int i = 0; i < maxConnections; i++)
		connectionPorts[i] = -1;

	netSocket = socket(AF_INET, SOCK_STREAM, TCP)	
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	//---SETUP SERVER---//
	bind(netSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));


}



void *ListenToNewConnections(){
	//{TODO} I don't think I'm supposed to just surround this in a straight up loop...
	while(true){
		listen(netSocket, maxConnections);
		int clientSocket = accept(netSocket, NULL, NULL);
		if(currentConnections < maxConnections){
			//Connection succesful
			send(clientSocket, connectionMessage, sizeof(connectionMessage));

		}else{
			//Connection declined
			send(clientSocket, declineMessage, sizeof(declineMessage));
		}
	}	
}

int AddConnection(int connectionPort){
	int emptyConnectionIndex = -1;
	
	//Find first element in 'connectionPorts'
	for(int i = 0; i < maxConnections; i++){
		if(connectionPorts[i] == -1){
			emptyConnectionIndex = -1;
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
void *RecieveMessage(){
	//{TODO} Clear from memory
	char recievedMessage[2048] = ""; //{TODO} Move buffer size into member field	
	while(true){//{TODO} Replace loop
		recv(netSocket, &recievedMessage, sizeof(recievedMessage));
		printf(recievedMessage);
	}
}

//To be run asyncronously
void *SendMessage(){
	//{TODO} Get user input
	//{TODO} Clear from memory
	char message[2048] = ""; //{TODO} Move buffer size into member field
	
}

