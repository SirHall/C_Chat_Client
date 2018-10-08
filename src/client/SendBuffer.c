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
    buffer = calloc(maxBufferSize, sizeof(char));
    Clear();
}

void AppendCharacter(char c){
    if(buffIndex >= maxBufferSize) return;
    buffer[buffIndex++] = c;
    ClampIndex();
}

void RemoveLastCharacter(){
    buffer[buffIndex--] = '\0';
    ClampIndex();
}

char* GetBuffer(){
    return buffer;
}

void Clear(){
    for(int i = 0; i < maxBufferSize; i++)
        buffer[i] = '\0';
    buffIndex = 0;
}
