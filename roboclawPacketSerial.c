#include <stdarg.h>
#include <stdint.h>
#include "roboclawPacketSerial.h"

// Defines
#define ADDRESS 128
#define TYPE_UINT8 0
#define TYPE_INT8 1
#define TYPE_UINT16 2
#define TYPE_INT16 3
#define TYPE_UINT32 4
#define TYPE_INT32 5

void sendCommand(char cmd, int numArgs, ...);
uint16_t crc16(unsigned char *packet, int nBytes);

void sendCommand(char cmd, int numArgs, ...) {
    va_list valist;
    va_start(valist, numArgs);
    int k = 0;
    
    char buffer[60];
    int index = 0;
    
    buffer[index++] = (char)(ADDRESS);
    buffer[index++] = cmd;
    
    for(k=0; k<numArgs; k++) {
        uint8_t type = va_arg(valist, uint8_t);
        
        if(type == TYPE_UINT8) {
            buffer[index++] = va_arg(valist, uint8_t);
        }
        else if(type == TYPE_INT8) {
            buffer[index++] = va_arg(valist, int8_t);
        }
        else if(type == TYPE_UINT16) {
            uint16_t value = va_arg(valist, uint16_t);
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_INT16) {
            int16_t value = va_arg(valist, int16_t);
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_UINT32) {
            uint32_t value = va_arg(valist, uint32_t);
            buffer[index++] = value >> 24;
            buffer[index++] = value >> 16;
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_INT32) {
            int32_t value = va_arg(valist, int32_t);
            buffer[index++] = value >> 24;
            buffer[index++] = value >> 16;
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
    }
    
    uint16_t crc = crc16((unsigned char*)buffer, index);
    
    buffer[index++] = crc >> 8;
    buffer[index++] = crc;
    
    printf("%s", buffer);
    
    va_end(valist);
}

//Calculates CRC16 of nBytes of data in byte array message
uint16_t crc16(unsigned char *packet, int nBytes) {
    uint16_t crc = 0;
    int byte = 0;
    for (byte = 0; byte < nBytes; byte++) {
        crc = crc ^ ((unsigned int)packet[byte] << 8);
        unsigned char bit = 0;
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } 
            else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

/*
 * Value must be between 0 and 127.
 */
void driveForwardM1(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(0, 1, TYPE_INT8, value );
}

void driveBackwardsM1(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(1, 1, TYPE_INT8, value);
}

void driveForwardM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(4, 1, TYPE_INT8, value);
}

void driveBackwardsM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(5, 1, TYPE_INT8, value);
}


