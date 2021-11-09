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

void loop(){
    cmd_update();
    wheel_update();
    lifter_update();    
}