#include"EndEffectorController.h"

void EndEffectorController::servoStartup(void)
{
  pwm = Adafruit_PWMServoDriver();
  pwm.begin();
  pwm.setPWMFreq(60);
}

void EndEffectorController::set_camera_mode(CamMode mode)
{
  switch(mode)
  {
    case LineFollow:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_LINE_FOLLOWING );//angle is a constant
    break;
    case PickUp:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_PICK_UP);
    break;
    case DropOff:
      pwm.setPWM(CAM_SERVO, 0, ANGLE_DROP_OFF);
    break;
  }
}
void EndEffectorController::set_clamp(int servo, int distance)
{
  if(distance > MAX_DISTANCE) 
  {
    pwm.setPWM(servo, 0, MAX_ANGLE);
    return ;
  }
   if(distance < MIN_DISTANCE) 
  {
    pwm.setPWM(servo, 0, MIN_ANGLE);
    return ;
  }
  pwm.setPWM(servo, 0, distance * 3 + 125 );
}
