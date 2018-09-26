#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#pragma region defines

#define TCP 0

#pragma endregion

int port = 9002;
char IPAddress[] = "127.0.0.1";

int maxConnections = 5;

int main(){
	
	char serverMessage[2048] = "You have reached the server";
	
	//Create the server socket
	int netSocket = socket(AF_INET, SOCK_STREAM, TCP);
	
	//Define the server address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	//Bind the socket to our specified IP and port
	bind(netSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	listen(netSocket, maxConnections);

	int clientSocket = accept(netSocket, NULL, NULL);
	
	//Send message to client
	send(clientSocket, serverMessage, sizeof(serverMessage), 0);
	
	//Close the socket
	close(netSocket);

	return 0;
}

