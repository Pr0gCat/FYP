#include "ClawController.h"
#include "Communication.h"
#include "WheelController.h"

ClawController claw;

void setup(){
    Serial.begin(115200);
    while(!Serial){;}
    Serial.println("start");
    delay(1000);

    wheel_setup();
    runSpeed(500, 500);
    // claw.begin();
}

void loop(){
    cmd_update();
    wheel_update();
    // claw.update();
}