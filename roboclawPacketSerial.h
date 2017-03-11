#include <stdint.h>

void roboclawPacketSerialPuttyInterface();

void sendCommand(char cmd, int numArgs, ...);
uint16_t crc16(unsigned char *packet, int nBytes);

void driveForwardM1(int8_t value);
void driveBackwardsM1(int8_t value);
void driveForwardM2(int8_t value);
void driveBackwardsM2(int8_t value);