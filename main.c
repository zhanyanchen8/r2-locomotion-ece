#include "config.h"
//#include "pt_cornell_1_2_1.h"
//#include "simplifiedSerial.h"
//#include "feedback.h"

//=============================================//
// Protothread structs
//=============================================//
//static struct pt pt_KangarooSerial;

//=============================================//
// Protothread functions
//=============================================//
//static PT_THREAD (protothread_KangarooSerial(struct pt *pt)) {
//    PT_BEGIN(pt);
//    while(1) {
//        incrementalPositionAndSpeed(LEFT_MOTOR, 1000, 500, PT_send_buffer);
//        //PutSerialBuffer(pt);
//    }
//    PT_END(pt);
//}
//
//static PT_THREAD (protothread_RoboclawSerial(struct pt *pt)) {
//    PT_BEGIN(pt);
//    while(1) {
//        PT_send_buffer[0] = 210;
//        PT_SPAWN(pt, &pt_output, PutSerialBuffer(&pt_output) );
//    } 
//    PT_END(pt);
//}
void configUART();
void configUART() {
    // config for putty
    
    // specify PPS group, signal, logical pin name
    PPSInput (2, U2RX, RPB8); //Assign U2RX to pin RPB11 -- Physical pin 17 on 28 PDIP
    PPSOutput(4, RPB9, U2TX); //Assign U2TX to pin RPB10 -- Physical pin 18 on 28 PDIP
    // init the uart2
    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, pb_clock, BAUDRATE);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
    mPORTBSetPinsDigitalIn(BIT_8);
    mPORTBSetPinsDigitalOut(BIT_9);
    
    // config for PUTTY
    // specify PPS group, signal, logical pin name
    PPSInput (3, U1RX, RPB13); //Assign U2RX to pin RPB11 -- Physical pin 24 on 28 PDIP
    PPSOutput(1, RPB15, U1TX); //Assign U2TX to pin RPB10 -- Physical pin 26 on 28 PDIP
    // init the uart2
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, pb_clock, BAUDRATE);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
    mPORTBSetPinsDigitalIn(BIT_13);
    mPORTBSetPinsDigitalOut(BIT_15);
}

int main(void) {
    // Schedule the protothreads
    
    //PT_setup();
    
    //PT_INIT(&pt_KangarooSerial);
    
//    while(1) {
//        PT_SCHEDULE(protothread_KangarooSerial(&pt_KangarooSerial));
//    }
    SYSTEMConfigPerformance(sys_clock);
    
    configUART();
    
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
            cmd = (unsigned char)atoi(buffer);
            index = 0;
            
            if(cmd < 0 || cmd > 255) {
                //print(&cmd, 1);
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

void print(char* buffer, int length) {
    int i = 0;
    for(i=0; i<length; i++) {
        while(!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, buffer[i]);
    }
}