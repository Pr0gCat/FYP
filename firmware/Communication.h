#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "config.h"

#define CMD_COFIRM 1
#define CMD_INIT 2
#define CMD_GET_CLAW_STATE 3
#define CMD_SET_CLAW_STATE 4
#define CMD_SET_LINEFOLLOW_MODE 5
#define CMD_SET_PICKUP_MODE 6
#define CMD_SET_DROPOFF_MODE 7
#define CMD_MOVE_POSY 8
#define CMD_MOVE_POSZ 9
#define CMD_GOTO_POSY 10 // absolute position
#define CMD_GOTO_POSZ 11
#define CMD_HOME_Y 12 // relative
#define CMD_HOME_Z 13
#define CMD_RUN_DISTANCE 14
#define CMD_GET_MOTOR_SPEED 15
#define CMD_SET_MOTOR_SPEED 16
#define CMD_MSG 255

void cmd_update();
void confirm(int cmd);

#endif