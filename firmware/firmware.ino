#include "LifterController.h"
#include "Communication.h"
#include "WheelController.h"

void setup(){
    Serial.begin(115200);
    wheel_setup();
    lifter_setup();

}

void loop(){
    cmd_update();
    wheel_update();
    lifter_update();    
}