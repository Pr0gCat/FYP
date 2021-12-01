//TODO: Cleanup inconsistent naming

#include "LifterController.h"
#include "Communication.h"
#include "WheelController.h"
#include "EndEffectorController.h"

void setup(){
    Serial.begin(115200);
    while(!Serial){}
    wheel_setup();
    lifter_setup();
    endeffector_setup();
}
static unsigned long t0 = 0;
void loop(){
    if(millis() - t0 > 500)
  {
    if(!Serial){
      stop_motor_l();
      stop_motor_r();
      lifter_stop();
    }
    t0 = millis();
  }
    cmd_update();
    wheel_update();
    lifter_update();    
}