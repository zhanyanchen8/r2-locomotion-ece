#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "hardwareConfig.h"

void configPuttyUART(){
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
}

void configRoboclawUART() {
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

void configMotorEncoders() {
    
}