#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "config.h"

// Arduino -> PC

#define CMD_COFIRM 1

#define CMD_GET_CRAWL_STATE 2
#define CMD_GET_CRAWL_TRIGGER 3
#define CMD_GET_POSY 4
#define CMD_GET_POSZ 5
#define CMD_GET_SPEED 6

#define CMD_LIMIT_TRIGGERED 7

typedef struct 
{
    uint8_t cmd;
    uint8_t length;

} PackageHeader_t;

#define CMD_SET_SPEED 8
typedef struct
{
    int16_t left;
    int16_t right;
} SetSpeedPayload_t;
#define PAYLOAD_SET_SPEED_SIZE sizeof(SetSpeedPayload_t)

#define CMD_MOVE_Y 9
#define CMD_MOVE_Z 10
typedef struct
{
    uint32_t pos;
} MoveClawPayload_t;
#define PAYLOAD_MOVE_CLAW_SIZE sizeof(MoveClawPayload_t)

#define CMD_HOME_Y 11
#define CMD_HOME_Z 12
#define CMD_MSG 255

void UART_send_msg(char *msg, ...);


#endif