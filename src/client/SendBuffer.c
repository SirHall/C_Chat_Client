#include <stdlib.h>
#include "SendBuffer.h"

char *buffer;
int maxBufferSize = 2048;
int buffIndex = 0;

void ClampIndex(){
	if(buffIndex >= maxBufferSize) buffIndex = maxBufferSize - 1;
	if(buffIndex < 1) buffIndex = 1;
}

void Initialize(){
	buffer = (char*) calloc(maxBufferSize, sizeof(char));
	// printf("Buffer size: %d\n", sizeof(buffer));
	Clear();
}

void AppendCharacter(char c){
	if(buffIndex >= maxBufferSize) return;
	ClampIndex();
	buffer[buffIndex++] = c;
}

void RemoveLastCharacter(){
	if(buffIndex >= 1)
		buffer[--buffIndex] = '\0';
}

char* GetBuffer(){
	return buffer;
}

int GetBufferSize(){
	return maxBufferSize;
}

void SetMessageType(char type){
	buffer[0] = type;
}

void Clear(){
	for(int i = 0; i < maxBufferSize; i++)
		buffer[i] = '\0';
	buffIndex = 1;
}
