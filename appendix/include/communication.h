#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

int readWord(int, int);
int readByte(int, int);
int getResult();
int getRXpacketError(int);
void writeWord(int,int,int);
void writeByte(int,int,int);
void pingID(int);

#endif
