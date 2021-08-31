#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "config.h"

typedef struct 
{
    uint8_t cmd;
    uint8_t length;

} PackageHeader_t;

#define CMD_COFIRM 1
#define CMD_GET_CRAWL_STATE 2
#define CMD_GET_CRAWL_TRIGGER 3
#define CMD_GET_POSY 4
#define CMD_GET_POSZ 5
#define CMD_GET_SPEED 6
#define CMD_LIMIT_TRIGGERED 7
#define CMD_SET_SPEED 8
#define CMD_MOVE_Y 10
#define CMD_MOVE_Z 11
#define CMD_HOME_Y 12
#define CMD_HOME_Z 13
#define CMD_MSG 255

void cmd_update();

#endif