#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

enum CamMode{line_following, pick_up, drop_off};

class frontServo
{
  public:
    Adafruit_PWMServoDriver pwm;
    void servoStartup(void);
    void set_camera_mode(CamMode mode);
    void set_clamp(int servo, int distance);
};
