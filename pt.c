#include "pt.h"
#include "stdint.h"

// Taken from Bruce Land's modification of protothreads, pt_cornell_1_2_1.h
// Modified by Manish Patel

volatile unsigned long long time_tick_millsec;

//void PT_YIELD_TIME_msec(int delay_time) {
//    do {                                                                \
//        static unsigned int time_thread ;                               \
//        time_thread = time_tick_millsec + (unsigned int)delay_time ;    \
//        PT_YIELD_UNTIL(pt, (time_tick_millsec >= time_thread));         \
//    } while(0);  
//}
//
//void PT_GET_TIME() {
//    return time_tick_millsec;
//}

