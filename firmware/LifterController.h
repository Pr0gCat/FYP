#ifndef _LIFTER_CONTROLLER_H_
#define _LIFTER_CONTROLLER_H_

#include "config.h"
#include <AccelStepper.h>

void lifter_setup();
void lifter_update();
void lifter_homeZ();
void lifter_homeY();

typedef enum {
    Y = 0,
    Z,
} AXIS;

template<AXIS axis>
void lifter_moveTo(uint32_t mm);
#endif