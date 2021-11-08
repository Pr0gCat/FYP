#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

#define CLAMP_L_SERVO 0
#define CLAMP_R_SERVO 1
#define CAM_SERVO 2

// angle to distance formula: distance*6/5
// distance to pwm: distance*3+125
#define MAX_DISTANCE 65
#define MAX_ANGLE 325 //angle = 80

// angle calculation formula: angle*5/2+125
#define ANGLE_LINE_FOLLOWING 200
#define ANGLE_PICKUP 350
#define ANGLE_LINE_DOCKING 575

enum CamMode{line_following, pickup, docking};

class frontServo
{
  public:
    Adafruit_PWMServoDriver pwm;
    void servoStartup(void);
    void set_camera_mode(CamMode mode);
    void set_clamp(int servo, int distance);
};

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
