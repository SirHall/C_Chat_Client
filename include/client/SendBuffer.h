#ifndef SendBuffer_h
#define SendBuffer_h

void Initialize();
void AppendCharacter(char c);
void RemoveLastCharacter();
char* GetBuffer();
void Clear();

#endif //SendBuffer.h