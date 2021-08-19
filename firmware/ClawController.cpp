#include "ClawController.h"

void ClawController::begin(){
    pinMode(ENDSTOP_Y_UPPER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Y_LOWER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Z_UPPER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Z_LOWER_PIN, INPUT_PULLUP);

    pinMode(STEPPER_Y_STEP_PIN, OUTPUT);
    pinMode(STEPPER_Y_DIR_PIN, OUTPUT);
    pinMode(STEPPER_Z_STEP_PIN, OUTPUT);
    pinMode(STEPPER_Z_DIR_PIN, OUTPUT);

    y_axis.setPinsInverted(true, false, false);
    y_axis.setMaxSpeed(STEPPER_HOMING_SPEED);
    y_axis.setAcceleration(STEPPER_HOMING_ACCEL);
    z_axis.setPinsInverted(true, false, false);
    z_axis.setMaxSpeed(STEPPER_HOMING_SPEED);
    z_axis.setAcceleration(STEPPER_HOMING_ACCEL);

    calibrateY();
    calibrateZ();
}

void ClawController::update(){
    y_axis.run();
    z_axis.run();
}

void ClawController::homeZ(){
    z_axis.moveTo(0);
    while(z_axis.distanceToGo() != 0 && digitalRead(ENDSTOP_Z_LOWER_PIN)){
        z_axis.run();
        if(!digitalRead(ENDSTOP_Z_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
}

void ClawController::homeY(){
    y_axis.moveTo(0);
    while(y_axis.distanceToGo() != 0 && digitalRead(ENDSTOP_Y_LOWER_PIN)){
        y_axis.run();
        if(!digitalRead(ENDSTOP_Y_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
}

void ClawController::calibrateY(){
    // go all the way down
    y_axis.setSpeed(-1000);
    while(digitalRead(ENDSTOP_Y_LOWER_PIN)){
        y_axis.runSpeed();
        if(!digitalRead(ENDSTOP_Y_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }

    // release Y lower limit
    y_axis.setSpeed(400);
    while(!digitalRead(ENDSTOP_Y_LOWER_PIN)){
        y_axis.runSpeed();
        if(digitalRead(ENDSTOP_Y_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    y_axis.setCurrentPosition(0);
    delay(500);

    // move all the way to the top
    y_axis.setSpeed(1000);
    while(digitalRead(ENDSTOP_Y_UPPER_PIN)){
        y_axis.runSpeed();
        if(!digitalRead(ENDSTOP_Y_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    delay(500);

    // release Y upper limit
    y_axis.setSpeed(-400);
    while(!digitalRead(ENDSTOP_Y_UPPER_PIN)){
        y_axis.runSpeed();
        if(digitalRead(ENDSTOP_Y_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    this->y_max = y_axis.currentPosition();
}

void ClawController::calibrateZ(){

    this->z_max = z_axis.currentPosition();
}