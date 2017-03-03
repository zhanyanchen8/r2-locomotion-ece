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

void configUART() {
    // specify PPS group, signal, logical pin name
    PPSInput (2, U2RX, RPB11); //Assign U2RX to pin RPB11 -- Physical pin 22 on 28 PDIP
    PPSOutput(4, RPB10, U2TX); //Assign U2TX to pin RPB10 -- Physical pin 21 on 28 PDIP
    // init the uart2
    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, pb_clock, BAUDRATE);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
    mPORTBSetPinsDigitalIn(BIT_11);
    mPORTBSetPinsDigitalOut(BIT_10);
    
    // config for PUTTY
    // specify PPS group, signal, logical pin name
    PPSInput (3, U1RX, RPA4); //Assign U2RX to pin RPB11 -- Physical pin 12 on 28 PDIP
    PPSOutput(1, RPB4, U1TX); //Assign U2TX to pin RPB10 -- Physical pin 11 on 28 PDIP
    // init the uart2
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, pb_clock, BAUDRATE);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
    mPORTASetPinsDigitalIn(BIT_4);
    mPORTBSetPinsDigitalOut(BIT_4);
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
    char c = 210;
    printf("%c", c);
    
    while(!UARTTransmitterIsReady(UART1));
    UARTSendDataByte(UART1, 'a');
    while(!UARTTransmitterIsReady(UART1));
    UARTSendDataByte(UART1, '\n');
    
    const int MAXLENGTH = 5;
    char buffer[MAXLENGTH];
    
    int i = 0;
    for(i=0; i<MAXLENGTH; i++) {
        buffer[i] = '\0';
    }
    
    unsigned char cmd;
    int index = 0;
    while(1) {
        while(!UARTReceivedDataIsAvailable(UART1));
        char rx = UARTGetDataByte(UART1);

        if(rx == 8) { // backspace
            while(!UARTTransmitterIsReady(UART1));
            UARTSendDataByte(UART1, 127);
            
            if(index > 0) {
                buffer[--index] = '\0';
            }
        }
        else if(rx == 13) {
            while(!UARTTransmitterIsReady(UART1));
            UARTSendDataByte(UART1, rx);
            
            while(!UARTTransmitterIsReady(UART1));
            UARTSendDataByte(UART1, '\n');
            
            buffer[index++] = '\0';
            cmd = (unsigned char)atoi(buffer);
            index = 0;
            
            if(cmd < 0 || cmd > 255) {
                //print(&cmd, 1);
            }
            else {
                printf("%c", (char)cmd);
                while(!UARTTransmitterIsReady(UART1));
                UARTSendDataByte(UART1, 'd');
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
                while(!UARTTransmitterIsReady(UART1));
                UARTSendDataByte(UART1, rx);
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