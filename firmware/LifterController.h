#ifndef _LIFTER_CONTROLLER_H_
#define _LIFTER_CONTROLLER_H_

#include "config.h"
#include <AccelStepper.h>

void lifter_setup();
void lifter_update();
void lifter_stop();
void lifter_homeZ();
void lifter_homeY();

void calibrateZ();
void calibrateY();

typedef enum {
    Y = 0,
    Z,
} AXIS;

void lifter_move(AXIS axis, long mm);
void lifter_move_relative(AXIS axis, long mm);
#endif