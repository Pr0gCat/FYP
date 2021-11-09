#ifndef _END_EFFECTOR_CONTROLLER_H_
#define _END_EFFECTOR_CONTROLLER_H_
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

enum CamMode {
  LineFollow = 1,
  PickUp = 2,
  DropOff = 3,
};

void endeffector_setup();
void set_camera_mode(CamMode mode);
void set_clamp(int servo, int distance);

#endif