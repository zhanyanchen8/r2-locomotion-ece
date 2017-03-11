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
int FindMax (int n, ...);
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
    
    int m;
    m= FindMax (7,702,422,631,834,892,104,772);
    printf ("The largest value is: %d\n",m);
    
    roboclawPacketSerialPuttyInterface();
}

int FindMax (int n, ...)
{
  int i,val,largest;
  va_list vl;
  va_start(vl,n);
  largest=va_arg(vl,int);
  for (i=1;i<n;i++)
  {
    val=va_arg(vl,int);
    largest=(largest>val)?largest:val;
  }
  va_end(vl);
  return largest;
}


