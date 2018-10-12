#ifndef SendBuffer_h
#define SendBuffer_h

void Initialize();
void AppendCharacter(char c);
void RemoveLastCharacter();
char* GetBuffer();
int GetBufferSize();
void Clear();

#endif //SendBuffer.h