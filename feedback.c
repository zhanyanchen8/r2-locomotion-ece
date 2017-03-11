#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "feedback.h"
#include "pt_cornell_1_2_1.h"

//=============================================//
// Defines
//=============================================//
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

//=============================================//
// Protothreads Structs
//=============================================//
static struct pt pt_leftPID;

//=============================================//
// Global Variables
//=============================================//
volatile int prevLeftMotor = 0;
volatile int currLeftMotor = 0;

volatile int prevRightMotor = 0;
volatile int currRightMotor = 0;

volatile int leftDirection = CLOCKWISE;
volatile int rightDirection = COUNTERCLOCKWISE;

float kP = 1.0;
float kI = 0.0;
float kD = 1.0;

/**
 * The motors used for locomotion on R2 have quadrature feedback. We connect
 * channel A on each motor to an input capture module; measuring the period
 * of the pulses gives the velocity of that motor. By connecting channel B of
 * each motor to a digital input pin and reading it during the ISR of the
 * input capture, we can determine the direction of rotation.
 */
void configureMotorEncoders() {
    // Open Timer 2, used as the source for the input capture modules
    //Prescalar of 1, other settings keep as default
    OpenTimer2(T2_ON | T2_PS_1_1, 65535);
    
    //=====================================//
    // Left Motor Configuration
    //=====================================//
    
    // Input Capture 1 on RPA2, pin 9
    // Channel A of quadrature encoder of left motor
    IC1R = 0;
    OpenCapture1(IC_ON | IC_IDLE_CON | IC_FEDGE_RISE | IC_CAP_16BIT | 
                 IC_TIMER2_SRC | IC_INT_1CAPTURE | IC_EVERY_RISE_EDGE);
    mIC1ClearIntFlag();
    mIC1SetIntPriority(1);
    mIC1IntEnable(1);
    
    // Connect Channel B on left motor to pin 10
    mPORTASetPinsDigitalIn(BIT_3);
    
    //=====================================//
    // Right Motor Configuration
    //=====================================//
    
    // Input Capture 2 on RPA3, pin 11
    // Channel A of quadrature encoder on right motor
    IC4R = 2;
    OpenCapture4(IC_ON | IC_IDLE_CON | IC_FEDGE_RISE | IC_CAP_16BIT | 
                 IC_TIMER2_SRC | IC_INT_1CAPTURE | IC_EVERY_RISE_EDGE);
    mIC4ClearIntFlag();
    mIC4SetIntPriority(2);
    mIC4IntEnable(1);
    
    // Connect Channel B on right motor to pin 12
    mPORTASetPinsDigitalIn(BIT_4);
}

//void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl1) IC1Interrupt(void) {
//    mIC1ClearIntFlag();
//    
//    prevLeftMotor = currLeftMotor;
//    currLeftMotor = mIC1ReadCapture();
//}

static PT_THREAD (protothread_leftPID(struct pt *pt)) {
    PT_BEGIN(pt);
    while(1) {
        // disable the interrupt while computing the new PID control value
        mIC1IntEnable(0);
        
        // re-enable the interrupt
        mIC1IntEnable(1);
    }
    PT_END(pt);
}