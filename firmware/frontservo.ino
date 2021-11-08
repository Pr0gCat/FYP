#include"frontservo.h"

void frontServo::servoStartup(void)
{
  pwm = Adafruit_PWMServoDriver();
  pwm.begin();
  pwm.setPWMFreq(60);
}

void frontServo::set_camera_mode(CamMode mode)
{
  switch(mode)
  {
    case line_following:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_LINE_FOLLOWING );//angle is a constant
    break;
    case pickup:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_PICKUP);
    break;
    case docking:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_LINE_DOCKING);
    break;
  }
}
void frontServo::set_clamp(int servo, int distance)
{
  if(distance > MAX_DISTANCE) 
    pwm.setPWM(servo, 0, MAX_ANGLE);
  else 
    pwm.setPWM(servo, 0, distance * 3 + 125 );
}

frontServo t;

void setup() {
  // put your setup code here, to run once:
  t.servoStartup();
}

void loop() {
  // put your main code here, to run repeatedly:
}
