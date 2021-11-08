#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class frontServo
{
  public:
    Adafruit_PWMServoDriver pwm;
    void servoStartup(void);
    void set_camera_mode(CamMode mode);
    void set_clamp(int servo, int distance);
};
