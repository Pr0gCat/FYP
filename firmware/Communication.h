#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "config.h"

#define CMD_COFIRM 1
#define CMD_INIT 2
#define CMD_SET_LINEFOLLOW_MODE 3
#define CMD_SET_PICKUP_MODE 4
#define CMD_SET_DROPOFF_MODE 5
#define CMD_MOVE_RELY 6
#define CMD_MOVE_RELZ 7
#define CMD_MOVE_ABSY 8
#define CMD_MOVE_ABSZ 9
#define CMD_STOP_Y 10
#define CMD_STOP_Z 11
#define CMD_HOME_Y 12
#define CMD_HOME_Z 13
#define CMD_RUN_DISTANCE 14
#define CMD_RUN_SPEED 15
#define CMD_MSG 255

void cmd_update();
void confirm(int cmd);
void send_msg(String str);
void send_msg(char[]);

#endif