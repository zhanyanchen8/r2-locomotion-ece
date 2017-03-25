/////////////////////////////////
// See settings in config.h
// set up clock parameters
// system cpu clock
#define sys_clock 40000000
#define pb_clock sys_clock // divide by one in this case

#define BAUDRATE 9600

void configPuttyUART();
void configRoboclawUART();
void configMotorEncoders();
