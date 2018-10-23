#ifndef SendBuffer_h
#define SendBuffer_h

void Initialize();
void AppendCharacter(char c);
void RemoveLastCharacter();
void SetMessageType(char type);
char* GetBuffer();
int GetBufferSize();
void Clear();

#endif //SendBuffer.h