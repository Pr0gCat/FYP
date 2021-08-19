#include "ClawController.h"
ClawController claw;

void setup(){
    claw.begin();
}

void loop(){
    claw.update();
}