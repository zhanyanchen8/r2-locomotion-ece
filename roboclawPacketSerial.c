#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
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

void roboclawPacketSerialPuttyInterface() {
    while(1) {
        while(!UARTTransmitterIsReady(UART2));
        printf("Ready to receive commands.\n");
    
        const int MAXLENGTH = 20;
        char buffer[MAXLENGTH];

        int i = 0;
        for(i=0; i<MAXLENGTH; i++) {
            buffer[i] = '\0';
        }

        unsigned char cmd;
        int index = 0;
        while(1) {
            while(!UARTReceivedDataIsAvailable(UART2));
            char rx = UARTGetDataByte(UART2);

            if(rx == 8) { // backspace
                // echo a delete character instead, since PUTTY executes backspace
                // as "move back one position but do not delete the character"
                while(!UARTTransmitterIsReady(UART2));
                UARTSendDataByte(UART2, 127); 

                if(index > 0) {
                    buffer[--index] = '\0';
                }
            }
            else if(rx == 13) {
                while(!UARTTransmitterIsReady(UART2));
                UARTSendDataByte(UART2, rx);

                while(!UARTTransmitterIsReady(UART2));
                UARTSendDataByte(UART2, '\n');

                buffer[index++] = '\0';
                
                int temp_cmd = atoi(&buffer[2]);
                cmd = (unsigned char)temp_cmd;
                index = 0;
                //printf(cmd);
                //printf("I made it this far!");
                
                if(buffer[0] != 'M' || !(buffer[1] == '1' || buffer[1] == '2')) {
                    printf("Not a valid command format. Print something of the regex M[1,2][int].\n");
                }
                else {
                    // Motor 1
                    if(buffer[1] == '1') {
                        driveM1SignedSpeed(temp_cmd);
                        int x = 0;
                        while(x < 1000000) {
                            x++;
                        }
                        readRawSpeedM1();
                    }
                    else { // Motor 2
                        driveM2SignedSpeed(temp_cmd);
                    }
                }

                for(i=0; i<MAXLENGTH; i++) {
                    buffer[i] = '\0';
                }
            }
            else {
                if(index == MAXLENGTH - 1) {
                    // do nothing, because the buffer is full
                }
                else {
                    while(!UARTTransmitterIsReady(UART2));
                    UARTSendDataByte(UART2, rx);
                    buffer[index++] = rx;
                }
            }
        }
    }
}

void sendCommand(char cmd, int numArgs, ...) {
    va_list valist;
    va_start(valist, numArgs);
    int k = 0;
    
    char buffer[60];
    int index = 0;
    
    buffer[index++] = (char)(ADDRESS);
    buffer[index++] = cmd;
    
    for(k=0; k<numArgs; k++) {
        int type = va_arg(valist, int);
        
        if(type == TYPE_UINT8) {
            buffer[index++] = (char)va_arg(valist, int);
        }
        else if(type == TYPE_INT8) {
            buffer[index++] = (char)va_arg(valist, int);
        }
        else if(type == TYPE_UINT16) {
            uint16_t value = (uint16_t)va_arg(valist, int);
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_INT16) {
            int16_t value = (int16_t)va_arg(valist, int);
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_UINT32) {
            uint32_t value = (uint32_t)va_arg(valist, int);
            buffer[index++] = value >> 24;
            buffer[index++] = value >> 16;
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
        else if(type == TYPE_INT32) {
            int32_t value = (int32_t)va_arg(valist, int);
            buffer[index++] = value >> 24;
            buffer[index++] = value >> 16;
            buffer[index++] = value >> 8;
            buffer[index++] = value;
        }
    }
    
    uint16_t crc = crc16((unsigned char*)buffer, index);
    
    buffer[index++] = crc >> 8;
    buffer[index++] = crc;
    
    // send the data byte
    int i=0;
    while(i < index) {
        while(!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, buffer[i]);
        i++;
    }
    
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
    sendCommand(0, 1, TYPE_INT8, (int)value );
}

void driveBackwardsM1(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(1, 1, TYPE_INT8, (int)value);
}

void driveForwardM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(4, 1, TYPE_INT8, (int)value);
}

void driveBackwardsM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(5, 1, TYPE_INT8, (int)value);
}

void driveM1SignedSpeed(int vel) {
    int QPPS = (int)(6144.0/22.0/63.5/3.14159*(float)(vel));
    printf("%d, QPPS\n");
    sendCommand(35, 1, TYPE_INT32, (int)vel);
}

void driveM2SignedSpeed(int vel) {
    int QPPS = (int)(6144.0/22.0/63.5/3.14159*(float)(vel));
    printf("%d, QPPS\n");
    sendCommand(36, 1, TYPE_INT32, (int)vel);
}

void readRawSpeedM1() {
    sendCommand(30, 0);
    
    int speed = 0;
    int i = 0;
    char buffer[4];
    for(i=0; i<7; i++) {
        while(!UARTReceivedDataIsAvailable(UART1));
        
        if(i == 0) {
            speed += ((uint32_t)(UARTGetDataByte(UART1)))<<4;
        }
        else if(i < 4) {
            speed += ((uint32_t)(UARTGetDataByte(UART1)))<<(4-i);
        }
        else {
            uint8_t temp = UARTGetDataByte(UART1);
        }
    }
    
    printf("Speed is: %d\n", speed*300);
    
}