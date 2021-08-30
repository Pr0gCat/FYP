#include "ClawController.h"
#include "WheelController.h"

ClawController claw;

void cmd_update();

void setup(){
    Serial.begin(115200);
    while(!Serial){;}
    Serial.println("start");
    delay(1000);

    wheel_setup();
    // claw.begin();
    set_speed_l(500);
    set_speed_r(500);
    set_distance_l(1000);
    set_distance_r(1000);
}

void loop(){
    cmd_update();
    // recive
    wheel_update();
    // claw.update();
}