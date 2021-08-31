#include "ClawController.h"
#include "Communication.h"
#include "WheelController.h"

ClawController claw;

void setup(){
    Serial.begin(115200);
    while(!Serial){;}
    Serial.println("start");

    wheel_setup();
    // claw.begin();
}

void loop(){
    // Serial.println("i");
    cmd_update();
    wheel_update();
    // claw.update();
}