#include "ClawController.h"

ClawController claw;

void cmd_update();

void setup(){
    Serial.begin(115200);
    while(!Serial){;}
    delay(3000);
    // wheels.begin();
    claw.begin();
}

void loop(){
    cmd_update();
    // recive
    // wheels.update();
    claw.update();
}