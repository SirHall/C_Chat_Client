#include "messageBuffer.h"

char **buffer; //Array of strings
int maxBufferElements = 2048;
int maxBufferElementLength = 2048;
int lostMessages = 0;

void Init(){
    buffer = calloc(maxBufferElements, maxBufferElementLength);
}

//Expected that input instance of string will no longer be used by caller
void Queue(char* message){
    int firstInvalid = FirstInvalid();
    if(firstInvalid == -1) { //We cannot add to queue
        lostMessages++; 
        free(message);
        return;
    }
    buffer[firstInvalid] = message;
}

char* Dequeue(){
    int firstValid = FirstValid();
    if(firstValid == -1) return '\0'; //There is no valid element
    char* newMessage = (char*)calloc(maxBufferElementLength, sizeof(char));
    strcpy(newMessage, buffer[firstValid]);
    InvalidateIndex(firstValid);
    return newMessage;
}

int IsIndexValid(int i){
    return buffer[i][0] != '\0';
}

void InvalidateIndex(int i){
    buffer[i][0] = '\0';
}

int IsAnyValid(){
    for(int i = 0; i < maxBufferElements; i++)
        if(IsIndexValid(i) == 1)
            return 1;
    return 0;
}

int FirstValid(){
    for(int i = 0; i < maxBufferElements; i++)
        if(IsIndexValid(i))
            return i;
    return -1;
}

int FirstInvalid(){
    for(int i = 0; i < maxBufferElements; i++)
        if(!IsIndexValid(i))
            return i;
    return -1;
}