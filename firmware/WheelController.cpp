#include "WheelController.h"

void WheelController::encoder_ticks_l(){
    // for distance
    if(digitalRead(ENCODER_LR_PIN)){
        encoder_pos_l++;
    }else{
        encoder_pos_l--;
    }
}

void WheelController::encoder_ticks_r(){
    // for distance
    if(digitalRead(ENCODER_RR_PIN)){
        encoder_pos_r++;
    }else{
        encoder_pos_r--;
    }
}

void WheelController::begin(){
    pinMode(ENCODER_LF_PIN, INPUT);
    pinMode(ENCODER_LR_PIN, INPUT);
    pinMode(ENCODER_RF_PIN, INPUT);
    pinMode(ENCODER_RR_PIN, INPUT);

    pinMode(MOTOR_L_PWM_PIN, OUTPUT);
    pinMode(MOTOR_L_DIR_PIN, OUTPUT);
    pinMode(MOTOR_R_PWM_PIN, OUTPUT);
    pinMode(MOTOR_R_DIR_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LF_PIN), (CallbackFunc)&encoder_ticks_l, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RF_PIN), (CallbackFunc)&encoder_ticks_r, RISING);

    speed_controller_l.begin();
    speed_controller_l.tune(SPEED_L_Kp, SPEED_L_Ki, SPEED_L_Kd);
    speed_controller_l.limit(-255, 255);

    speed_controller_r.begin();
    speed_controller_r.tune(SPEED_R_Kp, SPEED_R_Ki, SPEED_R_Kd);
    speed_controller_r.limit(-255, 255);

    distance_controller_l.begin();
    distance_controller_l.tune(DIST_L_Kp, DIST_L_Ki, DIST_L_Kd);
    distance_controller_l.limit(-255, 255);

    distance_controller_r.begin();
    distance_controller_r.tune(DIST_R_Kp, DIST_R_Ki, DIST_R_Kd);
    distance_controller_r.limit(-255, 255);
}

void WheelController::update(){
    uint32_t now = millis();
    uint32_t duration = now < last_seen ? (UINT32_MAX - last_seen + now) \
                        : (now - last_seen);
    if(duration < REFRESH_TIME) return;

    // in ticks per second
    int32_t current_tps_l = (encoder_pos_l - prev_pos_l) / duration / 1000;
    int32_t current_tps_r = (encoder_pos_r - prev_pos_r) / duration / 1000;
    
    if(distance_to_go != 0){
        // we are moving in distance mode
        double desire_speed_l = distance_controller_l.compute(encoder_pos_l);
        double desire_speed_r = distance_controller_r.compute(encoder_pos_r);

        speed_controller_l.setpoint(desire_speed_l);
        speed_controller_r.setpoint(desire_speed_r);
    }
    // calculate finite speed
    double pwm_l = speed_controller_l.compute(current_tps_l);
    double pwm_r = speed_controller_r.compute(current_tps_r);
    //apply speed to driver
    
    // save encoder position
    prev_pos_l = encoder_pos_l;
    prev_pos_r = encoder_pos_r;
}

// move the car with distance
void WheelController::moveTo(int32_t dist_l, int32_t dist_r){

}

// move the car with constant speed
void WheelController::runSpeed(double tps_l, double tps_r){
    speed_controller_l.setpoint(tps_l);
    speed_controller_r.setpoint(tps_r);
    distance_to_go = 0;
}

void WheelController::stop(){
    speed_controller_l.setpoint(0);
    speed_controller_r.setpoint(0);
    distance_to_go = 0;
}