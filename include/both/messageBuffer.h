#ifndef messageBuffer_h
#define messageBuffer_h

#include <string.h>
#include <stdlib.h>

void Init();

void Queue(char* message);
char* DeQueue();

int IsIndexValid(int i);
void InvalidateIndex(int i);
int IsAnyValid();
int FirstValid();
int FirstInvalid();

#endif //messageBuffer.h