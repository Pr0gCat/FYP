#include "LifterController.h"
#include "Communication.h"

AccelStepper y_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Y_STEP_PIN, STEPPER_Y_DIR_PIN);
AccelStepper z_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Z_STEP_PIN, STEPPER_Z_DIR_PIN);
uint32_t y_max = 0;
uint32_t z_max = 0;
bool z_running = false, y_running = false;
bool move_rel = false;
void calibrateZ();
void calibrateY();

void lifter_setup(){

    pinMode(ENDSTOP_Y_UPPER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Y_LOWER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Z_UPPER_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_Z_LOWER_PIN, INPUT_PULLUP);

    pinMode(STEPPER_Y_STEP_PIN, OUTPUT);
    pinMode(STEPPER_Y_DIR_PIN, OUTPUT);
    pinMode(STEPPER_Z_STEP_PIN, OUTPUT);
    pinMode(STEPPER_Z_DIR_PIN, OUTPUT);

    y_axis.setPinsInverted(false, false, true);
    y_axis.setAcceleration(STEPPER_MOVE_ACCEL);
    y_axis.setMaxSpeed(STEPPER_Y_MAX_SPEED);

    z_axis.setPinsInverted(true, false, true);
    z_axis.setAcceleration(STEPPER_MOVE_ACCEL);
    z_axis.setMaxSpeed(STEPPER_Z_MAX_SPEED);
}

void lifter_update(){
    if(!digitalRead(ENDSTOP_Y_UPPER_PIN) && y_axis.targetPosition() > y_max){
        y_max = y_axis.currentPosition();
        y_axis.setCurrentPosition(y_axis.currentPosition());
    }
    if(!digitalRead(ENDSTOP_Y_LOWER_PIN) && y_axis.targetPosition() < 0){ // OMFG!!!!
        // send_msg(String("targetY:" + String(y_axis.targetPosition())));
        send_msg("triggered");
        y_axis.setCurrentPosition(0);
    }
    if(!digitalRead(ENDSTOP_Z_UPPER_PIN) && z_axis.targetPosition() > z_max){
        z_max = z_axis.currentPosition();
        z_axis.setCurrentPosition(z_axis.currentPosition());
    }
    if(!digitalRead(ENDSTOP_Z_LOWER_PIN) && z_axis.targetPosition() < 0){
        z_axis.setCurrentPosition(0);
    }

    if(y_running && !y_axis.isRunning()){
        confirm(move_rel ? CMD_MOVE_RELY : CMD_MOVE_ABSY);
        y_running = false;
    }

    if(z_running && !z_axis.isRunning()){
        confirm(move_rel ? CMD_MOVE_RELZ : CMD_MOVE_ABSZ);
        z_running = false;
    }

    y_axis.run();
    z_axis.run();
}

void lifter_stop(){
    y_axis.setCurrentPosition(y_axis.currentPosition());
    z_axis.setCurrentPosition(y_axis.currentPosition());
    y_running = false;
    z_running = false;
}

void lifter_homeZ(){
    z_axis.setSpeed(-4000);
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
    confirm(CMD_HOME_Z);
}

void lifter_homeY(){
    y_axis.setSpeed(-3000);
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
    confirm(CMD_HOME_Y);
}

void calibrateY(){

    y_axis.setSpeed(-3000);
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
    y_axis.setSpeed(5000);
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
    y_max = y_axis.currentPosition();
}

void calibrateZ(){

    z_axis.setSpeed(-4000);
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
    z_axis.setSpeed(5000);
    while(digitalRead(ENDSTOP_Z_UPPER_PIN)){
        z_axis.runSpeed();
        if(!digitalRead(ENDSTOP_Z_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    
    // release Z upper limit
    // NOTE: some how the z_axis is still triggering the upper limit,
    // so we need to move it back a bit
    z_axis.setSpeed(-400);
    while(!digitalRead(ENDSTOP_Z_UPPER_PIN)){
        z_axis.runSpeed();
        if(digitalRead(ENDSTOP_Z_UPPER_PIN)){ delay(ENDSTOP_DEBRONCE_TIME); }
    }
    
    z_max = z_axis.currentPosition();
}

void lifter_move_rel(AXIS axis, long mm)
{
    move_rel = true;
    long pos = mm * MICROSTEP * STEPS_PER_REV / SCREW_LEAD;
    if(axis == Y){
        y_running = true;
        y_axis.move(pos);
    }else if (axis == Z){
        z_running = true;
        z_axis.move(pos);
    }
    
}

void lifter_move_abs(AXIS axis, long mm)
{
    move_rel = false;
    long pos = mm * MICROSTEP * STEPS_PER_REV / SCREW_LEAD;
    // Serial.println(pos);
    if(axis == Y){
        pos = pos > y_max ? y_max : pos;
        y_running = true;
        y_axis.moveTo(pos);
    }else if (axis == Z){
        pos = pos > z_max ? z_max : pos;
        z_running = true;
        z_axis.moveTo(pos);
    }
}

