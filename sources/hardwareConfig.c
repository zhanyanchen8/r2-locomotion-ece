#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "../includes/hardwareConfig.h"

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

void configEscon(void){
    /*-------------------
     * LED1 = RB14 = EN2
     * LED2 = RB13 = PWM2 (OC4)
     * LED3 = RB9 = ENABLE
     * LED4 = RB8 = PWM1 (OC2)
     * LED5 = RB7 = EN1
     *-------------------
     * PWM1 = RB8, OC2
     * PWM2 = RB13, OC4
     * EN1 = RB7
     * EN2 = RB14
     * ENABLE = RB9
     *------------------- 
     */
    OpenTimer3(T3_ON | T3_PS_1_1, 4999);
    OpenOC2(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenOC4(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    mPORTBSetPinsDigitalOut(BIT_14 | BIT_7 | BIT_9 | BIT_8 | BIT_13);
    mPORTBClearBits(BIT_14 | BIT_7 | BIT_9);
    PPSOutput(2, RPB8, OC2);
    PPSOutput(3, RPB13, OC4);
//    mPORTBSetBits(BIT_9);
}

void driveEscon(int channel, int speed){
    if (speed == 0){
        mPORTBClearBits(BIT_9);
        speed = 0;
    }
    else{
        mPORTBSetBits(BIT_9);
    }
    if (channel == 1){
        if (speed > 0){
            mPORTBSetBits(BIT_7);
            SetDCOC2PWM(speed);
        }
        else{
            mPORTBClearBits(BIT_7);
            SetDCOC2PWM(-speed);
        }
    }
    else if (channel == 2){
        if (speed > 0){
            mPORTBSetBits(BIT_14);
            SetDCOC4PWM(speed);
        }
        else{
            mPORTBClearBits(BIT_14);
            SetDCOC4PWM(-speed);
        }
    }
}

void configMotorEncoders() {
    
}
