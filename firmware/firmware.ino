#include "ClawController.h"

ClawController claw;

void setup(){
    Serial.begin(115200);
    while(!Serial){;}
    Serial.println("Startup");
    delay(3000);
    //wheels.begin();
    claw.begin();
}

void loop(){
    // recive
    // wheels.update();
    claw.update();
}