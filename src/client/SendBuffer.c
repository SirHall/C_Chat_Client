#include <stdlib.h>
#include "SendBuffer.h"

char *buffer;
int maxBufferSize = 2048;
int buffIndex = 0;

void ClampIndex(){
	if(buffIndex >= maxBufferSize) buffIndex = maxBufferSize - 1;
	if(buffIndex < 0) buffIndex = 0;
}

void Initialize(){
	buffer = (char*) calloc(maxBufferSize, sizeof(char));
	// printf("Buffer size: %d\n", sizeof(buffer));
	Clear();
}

void AppendCharacter(char c){
	if(buffIndex >= maxBufferSize) return;
	buffer[buffIndex++] = c;
	ClampIndex();
}

void RemoveLastCharacter(){
	buffer[--buffIndex] = '\0';
	ClampIndex();
}

char* GetBuffer(){
	return buffer;
}

int GetBufferSize(){
	return maxBufferSize;
}

void Clear(){
	for(int i = 0; i < maxBufferSize; i++)
		buffer[i] = '\0';
	buffIndex = 0;
}
