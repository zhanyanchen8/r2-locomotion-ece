#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "config.h"
#include "hardwareConfig.h"
#include "roboclawPacketSerial.h"
#include <stdarg.h>
//#include "pt_cornell_1_2_1.h"

//=============================================//
// Protothread structs
//=============================================//

//=============================================//
// Protothread functions
//=============================================//
int main(void) {
    // Schedule the protothreads
    
    //PT_setup();
    
    //PT_INIT(&pt_KangarooSerial);
    
//    while(1) {
//        PT_SCHEDULE(protothread_KangarooSerial(&pt_KangarooSerial));
//    }
    SYSTEMConfigPerformance(sys_clock);
    
    configPuttyUART();
    configRoboclawUART();
    
    roboclawUARTInterface();
    while(1) {
        //moveSquare();
    }
}