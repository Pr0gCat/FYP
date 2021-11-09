#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "config.h"

#define CMD_COFIRM 1
#define CMD_SYNC 2
#define CMD_INIT 3
#define CMD_GET_CLAW_STATE 4
#define CMD_SET_CLAW_STATE 5
#define CMD_SET_LINEFOLLOW_MODE 6
#define CMD_SET_PICKUP_MODE 7
#define CMD_SET_DROPOFF_MODE 8
#define CMD_GET_POSY 9
#define CMD_GET_POSZ 10
#define CMD_MOVE_POSY 11
#define CMD_MOVE_POSZ 12
#define CMD_HOME_Y 13
#define CMD_HOME_Z 14
#define CMD_RUN_DISTANCE 15
#define CMD_GET_MOTOR_SPEED 16
#define CMD_SET_MOTOR_SPEED 17
#define CMD_MSG 255

void cmd_update();

#endif