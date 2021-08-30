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

    y_axis.setPinsInverted(true, false, true);
    y_axis.setAcceleration(STEPPER_MOVE_ACCEL);
    y_axis.setMaxSpeed(STEPPER_Y_MAX_SPEED);

    z_axis.setPinsInverted(true, false, true);
    z_axis.setAcceleration(STEPPER_MOVE_ACCEL);
    z_axis.setMaxSpeed(STEPPER_Z_MAX_SPEED);

    calibrateZ();
    calibrateY();
    Serial.println("Calibration done");
    homeZ();
    homeY();
}

void ClawController::update(){
    // unexpected collision
    if(!digitalRead(ENDSTOP_Y_UPPER_PIN)){
        y_axis.setCurrentPosition(y_axis.currentPosition());
        y_max = y_axis.currentPosition();
    }
    if(!digitalRead(ENDSTOP_Y_LOWER_PIN)){
        y_axis.setCurrentPosition(0);
    }
    if(!digitalRead(ENDSTOP_Z_UPPER_PIN)){
        z_axis.setCurrentPosition(z_axis.currentPosition());
        z_max = z_axis.currentPosition();
    }
    if(!digitalRead(ENDSTOP_Z_LOWER_PIN)){
        z_axis.setCurrentPosition(0);
    }
    y_axis.run();
    z_axis.run();
}

void ClawController::homeZ(){
    z_axis.moveTo(0);
}

void ClawController::homeY(){
    y_axis.moveTo(0);
}

void ClawController::calibrateY(){

    y_axis.setSpeed(-1500);
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
    y_axis.setSpeed(1500);
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

    z_axis.setSpeed(-1500);
    while(digitalRead(ENDSTOP_Z_LOWER_PIN)){
        z_axis.runSpeed();
        if(!digitalRead(ENDSTOP_Z_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }

    // release Z lower limit
    z_axis.setSpeed(400);
    while(!digitalRead(ENDSTOP_Z_LOWER_PIN)){
        z_axis.runSpeed();
        if(digitalRead(ENDSTOP_Z_LOWER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    z_axis.setCurrentPosition(0);
    delay(500);

    // move all the way to the top
    z_axis.setSpeed(1500);
    while(digitalRead(ENDSTOP_Z_UPPER_PIN)){
        z_axis.runSpeed();
        if(!digitalRead(ENDSTOP_Z_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    delay(500);
    // release Y upper limit
    z_axis.setSpeed(-400);
    while(!digitalRead(ENDSTOP_Z_UPPER_PIN)){
        z_axis.runSpeed();
        if(digitalRead(ENDSTOP_Z_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    this->z_max = z_axis.currentPosition();
}

template<ClawController::AXIS axis>
void ClawController::moveTo(uint32_t mm){
    uint32_t pos = mm * MICROSTEP * STEPS_PER_REV / SCREW_LEAD;
    if(axis == ClawController::Y){
        pos = pos > y_max ? y_max : pos;
        y_axis.moveTo(pos);
    }else if (axis == ClawController::Z){
        pos = pos > z_max ? z_max : pos;
        y_axis.moveTo(pos);
    }
    return;
}
