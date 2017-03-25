#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include "roboclawPacketSerial.h"

// Defines
#define ADDRESS 128
#define TYPE_UINT8 0
#define TYPE_INT8 1
#define TYPE_UINT16 2
#define TYPE_INT16 3
#define TYPE_UINT32 4
#define TYPE_INT32 5

// Robot defines
#define CIRCUMFERENCE (2*3.14159*63.5) // in mm
#define TURNING_RADIUS 254.0 // in mm

void moveSquare() {
    driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, 1000, CIRCUMFERENCE, 1000);
    turn(3.14159/2.0, CIRCUMFERENCE/4);
    driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, 1000, CIRCUMFERENCE, 1000);
    turn(3.14159/2.0, CIRCUMFERENCE/4);
    driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, 1000, CIRCUMFERENCE, 1000);
    turn(3.14159/2.0, CIRCUMFERENCE/4);
    driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, 1000, CIRCUMFERENCE, 1000);
    turn(3.14159/2.0, CIRCUMFERENCE/4);
}

void roboclawUARTInterface() {
    while(1) {
        while(!UARTTransmitterIsReady(UART2));
        printf("Ready to receive commands.\n");
    
        // buffer to read in commands
        const int MAXLENGTH = 14;
        char buffer[MAXLENGTH];
        int index = 0;
        
        // Watchdog timer timeout in timer ticks
        const int TIMEOUT = 65000;

        while(1) {
            
            int i = 0;
            int timedOut = 0;
            index = 0;
            clearBuffer(buffer, MAXLENGTH);
            
            for(i=0; i<MAXLENGTH-2; i++) {
                // Start watchdog timer, 4 times longer than baud rate
                OpenTimer5(T5_ON, TIMEOUT);
                TMR5 = 0;
                
                // Wait for data or timer timeout
                while(!UARTReceivedDataIsAvailable(UART2) && ReadTimer5() < TIMEOUT);
                
                // if timeout occurred, clear the buffer, wait for a new packet
                if(ReadTimer5() >= TIMEOUT) {
                    timedOut = 1;
                    break;
                }
                
                // otherwise, read the data
                char rx = UARTGetDataByte(UART2);
                buffer[index++] = rx;
            }
            
            // if time out occurs, drop the packet
            if(timedOut) {
                //printf("Time out error\n");
                continue;
            }
            else {
                // send packet back
                buffer[12] = '\n';
                buffer[13] = 0;
                printf(buffer);
                
                // parse the packet
                if(buffer[0] != 'M' || buffer[6] != 'M' || buffer[1] !='1' || buffer[7] != '2') {
                    // packet structure incorrect, do nothing
                }
                else {
                    char M1speed[5];
                    char M2speed[5];
                    memcpy(M1speed, &buffer[2], 4);
                    memcpy(M2speed, &buffer[8], 4);
                    M1speed[4] = 0;
                    M2speed[4] = 0;
                    
                    int speedM1 = atoi(M1speed);
                    int speedM2 = atoi(M2speed);
                    
                    printf("%d\n", speedM1);
                    printf("%d\n", speedM2);
                    
                    if(speedM1 > 250 || speedM2 > 250) {
                        continue;
                    }
                    
                    driveM1SignedSpeed(speedM1);
                    driveM2SignedSpeed(speedM2);
                }
            }
        }
        
    }
}

void roboclawPacketSerialPuttyInterface() {
    while(1) {
        while(!UARTTransmitterIsReady(UART2));
        //printf("Ready to receive commands.\n");
    
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
                        //driveM1SignedSpeed(temp_cmd);
                        //driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, CIRCUMFERENCE/5.0, CIRCUMFERENCE, CIRCUMFERENCE/5.0);
                        turn(3.14159/2.0, CIRCUMFERENCE);
                    }
                    else { // Motor 2
                        //driveM2SignedSpeed(temp_cmd);
                        driveM1M2SignedSpeedAndDist(CIRCUMFERENCE, 2.0*CIRCUMFERENCE, CIRCUMFERENCE, 2.0*CIRCUMFERENCE);
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
    char buffer[1];
    readResponse(1, buffer);
}

void driveBackwardsM1(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(1, 1, TYPE_INT8, (int)value);
    char buffer[1];
    readResponse(1, buffer);
}

void driveForwardM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(4, 1, TYPE_INT8, (int)value);
    char buffer[1];
    readResponse(1, buffer);
}

void driveBackwardsM2(int8_t value) {
    if(value < 0 || value > 127) { return; }
    sendCommand(5, 1, TYPE_INT8, (int)value);
    char buffer[1];
    readResponse(1, buffer);
}

void driveM1SignedSpeed(int vel) {
    int QPPS = -(int)(60.0*6144.0/22.0/63.5/3.14159*(float)(vel));
    printf("%d, QPPS\n");
    sendCommand(35, 1, TYPE_INT32, QPPS);
    char buffer[1];
    readResponse(1, buffer);
}

void driveM2SignedSpeed(int vel) {
    int QPPS = -(int)(60.0*6144.0/22.0/63.5/3.14159*(float)(vel));
    printf("%d, QPPS\n");
    sendCommand(36, 1, TYPE_INT32, QPPS);
    char buffer[1];
    readResponse(1, buffer);
}

/*
 * Velocity in mm/s, distance in mm. This command is buffered, so if at the end
 * of execution the motor is still moving it will decelerate to zero velocity
 * over 1 second. In other words, it does not compute distance traveled during
 * the down ramp to reach the desired position. Solve this problem by adding
 * your own ramp.
 */
void driveM1M2SignedSpeedAndDist(int velM1, int distM1, int velM2, int distM2) {
    int QPPS1 = -(int)(368640.0/11/CIRCUMFERENCE*(float)(velM1));
    int distQP1 = (int)(368640.0/11/CIRCUMFERENCE*(float)(distM1));
    int QPPS2 = -(int)(368640.0/11/CIRCUMFERENCE*(float)(velM2));
    int distQP2 = (int)(368640.0/11/CIRCUMFERENCE*(float)(distM2));
        
    // Send first command
    sendCommand(43, 5, TYPE_INT32, QPPS1, TYPE_INT32, distQP1, 
            TYPE_INT32, QPPS2, TYPE_INT32, distQP2, TYPE_UINT8, 0);
    
    // Send first command
    sendCommand(43, 5, TYPE_INT32, 0, TYPE_INT32, 0, 
            TYPE_INT32, 0, TYPE_INT32, 0, TYPE_UINT8, 0);
    
    
    char buffer[1];
    readResponse(1, buffer);
    readResponse(1, buffer);
    
    printf("Desired distance was %d QP.\n", distQP1);
}

void readRawSpeedM1() {
    sendCommand(30, 0);
    
    int speed = 0;
    int i = 0;
    char buffer[7];
    
    readResponse(7, buffer);
    
    for(i=0; i<4; i++) {
        speed += ((uint32_t)(buffer[i]))<<(3-i);
    }
    
    if(buffer[4] == 1)
        speed *= -1;
    
    printf("Speed is: %d\n", speed*300);
}

/*
 * Executes a point turn through an angle of theta. Positive theta is CCW,
 * negative theta is CW. Theta specified in radians.
 */
void turn(float theta, int vel) {
    int dist = (int)(TURNING_RADIUS*fabs(theta));
    
    driveM1M2SignedSpeedAndDist(-vel,dist,vel,dist);
}

void clearUART1() {
    char rx = 0;
    rx = UARTGetDataByte(UART1);
    rx = UARTGetDataByte(UART1);
    rx = UARTGetDataByte(UART1);
}

/*
 * Read the packet sent by the Roboclaw using a 1 ms timeout for each byte
 */
void readResponse(int length, uint8_t* buffer) {
    OpenTimer5(T5_ON, 65535);
    TMR5 = 0;
    
    int i=0;
    for(i=0; i<length; i++) {
        while(!UARTReceivedDataIsAvailable(UART1) && ReadTimer5() < 65000);
        if(ReadTimer5() > 65000) {
            printf("Packet response timeout.\n");
            break;
        }
        
        buffer[i] = UARTGetDataByte(UART1);
        
        TMR5 = 0;
    }
    
    CloseTimer5();
}

void clearBuffer(char* buffer, int length) {
    int i = 0;
    for(i=0; i<length; i++) {
        buffer[i] = 0;
    }
}