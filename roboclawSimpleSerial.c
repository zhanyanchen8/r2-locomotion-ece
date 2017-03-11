#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "roboclawSimpleSerial.h"


void puttyInterfaceSimpleSerial() {
    while(!UARTTransmitterIsReady(UART2));
    printf("Ready to receive commands.\n");
    
    const int MAXLENGTH = 5;
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
            int temp_cmd = atoi(buffer);
            cmd = (unsigned char)temp_cmd;
            index = 0;
            
            if(temp_cmd < 0 || temp_cmd > 255) {
                printf("Please input a number between 0 and 255.\n");
            }
            else {
                while(!UARTTransmitterIsReady(UART1));
                UARTSendDataByte(UART1, cmd);
            }
            
            
            for(i=0; i<MAXLENGTH; i++) {
                buffer[i] = '\0';
            }
        }
        else {
            if(index == 3 || rx < 48 || rx > 57) {
                // do nothing, because either the buffer is full
                // or character is NAN
            }
            else {
                while(!UARTTransmitterIsReady(UART2));
                UARTSendDataByte(UART2, rx);
                buffer[index++] = rx;
            }
        }
    }
}