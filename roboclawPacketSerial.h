#include <stdint.h>

void roboclawUARTInterface();
void roboclawPacketSerialPuttyInterface();

void sendCommand(char cmd, int numArgs, ...);
uint16_t crc16(unsigned char *packet, int nBytes);

void driveForwardM1(int8_t value);
void driveBackwardsM1(int8_t value);
void driveForwardM2(int8_t value);
void driveBackwardsM2(int8_t value);
void driveM1SignedSpeed(int vel);
void driveM2SignedSpeed(int vel);
void readRawSpeedM1();
void driveM1M2SignedSpeedAndDist(int velM1,int distM1, int velM2, int distM2);
void turn(float theta, int vel);
void moveSquare();

void clearUART1();
void clearBuffer(char* buffer, int length);
void readResponse();