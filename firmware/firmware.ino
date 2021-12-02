#include "LifterController.h"
#include "Communication.h"
#include "WheelController.h"
#include "EndEffectorController.h"

void setup(){
    Serial.begin(115200);
    while(!Serial){}
    delay(1000);
    wheel_setup();
    lifter_setup();
    endeffector_setup();
}

static unsigned long t0 = 0;
void loop(){
    if(millis() - t0 > 500) // update every 0.5s
  {
    // the !Serial is too costly, do not do it every loop!
    if(!Serial){
      // this will be called when serial connection is lost
      stop_motor_l();
      stop_motor_r();
      lifter_stop(); // useless on homing
    }
    t0 = millis();
  }
    cmd_update();
    wheel_update();
    lifter_update();
}