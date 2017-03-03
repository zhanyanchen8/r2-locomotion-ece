/**
 * simplifiedSerial.c
 * 
 * Description: Every function in this file will
 * take in at least a character array as input,
 * and fill this array with a string representing
 * the appropriate simplified serial command. For
 * example, the position() function takes in a
 * parameter of the character array as well an
 * int pos for the desired position that should be
 * moved to, and the desired channel number (either
 * 1 or 2);
 */
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>

//=============================================//
// Setup Commands
//=============================================//

void start(int channel, char* command) {
    sprintf(command, "%d,start\r\n", channel);
}

void units(int channel, char* command) {
    sprintf(command, "%d,start\r\n", channel);
}

void home(int channel, char* command) {
    sprintf(command, "%d,home\r\n", channel);
}


//=============================================//
// Motion Commands
//=============================================//

/**
 * @param channel
 * @param pos
 * @param command
 * Fills command with a simplified serial command to
 * move the specified channel to the desired absolute 
 * position.
 */
void position(int channel, int pos, char* command) {
    sprintf(command, "%d,p%d\r\n", channel, pos);
}

/**
 * @param channel
 * @param speed
 * @param command
 * Fills command with a simplified serial command to
 * set the specified to the desired speed.
 */
void speed(int channel, int speed, char* command) {
    sprintf(command, "%d,s%d\r\n", channel, speed);
}

/**
 * @param channel
 * @param incPos
 * @param command
 * Fills command with a simplified serial command to
 * change the specified channel position by the desired
 * increment.
 */
void incrementalPosition(int channel, int incPos, char* command) {
    sprintf(command, "%d,pi%d\r\n", channel, incPos);
}

/**
 * @param channel
 * @param incSpeed
 * @param command
 * Fills command with a simplified serial command to
 * change the specified channel speed by the desired
 * increment.
 */
void incrementalSpeed(int channel, int incSpeed, char* command) {
    sprintf(command, "%d,si%d\r\n", channel, incSpeed);
}

/**
 * @param channel
 * @param pos
 * @param speed
 * @param command
 * Fills command with a simplified serial command to
 * move the specified channel to the desired position
 * at the desired speed.
 */
void positionAndSpeed(int channel, int pos, int speed, char* command) {
    sprintf(command, "%d,p%d s%d\r\n", channel, pos, speed);
}

/**
 * @param channel
 * @param incPos
 * @param speed
 * @param command
 * Fills command with a simplified serial command to
 * change the specified channel position by the desired
 * increment at the desired speed.
 */
void incrementalPositionAndSpeed(int channel, int incPos, int speed, char* command) {
    sprintf(command, "%d,pi%d s%d\r\n", channel, incPos, speed);
}

//=============================================//
// Readback Commands
//=============================================//

void getPosition(char* command) {
    sprintf(command, "1,getp\r\n");
}

void getSpeed(char* command) {
    sprintf(command, "1,gets\r\n");
}

