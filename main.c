#include "config.h"
#include "hardwareConfig.h"
#include "roboclawSimpleSerial.h"
//#include "pt_cornell_1_2_1.h"
//#include "simplifiedSerial.h"
//#include "feedback.h"

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
    
    configUART();
    
    puttyInterfaceSimpleSerial();
}