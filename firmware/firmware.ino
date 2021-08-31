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
    set_speed_l(500);
    set_speed_r(500);
    set_distance_l(1000);
    set_distance_r(1000);
    claw.begin();
}

void loop(){
    cmd_update();
    wheel_update();
    claw.update();
}