#include "WheelController.h"

void WheelController::encoder_ticks_l(){
    if(digitalRead(ENCODER_LF_PIN)){
        encoder_pos_l++;
    }else{
        encoder_pos_l--;
    }
}

void WheelController::encoder_ticks_r(){
    if(digitalRead(ENCODER_RF_PIN)){
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

    pinMode(MOTOR_L_IN1_PIN, OUTPUT);
    pinMode(MOTOR_L_IN2_PIN, OUTPUT);
    pinMode(MOTOR_R_IN1_PIN, OUTPUT);
    pinMode(MOTOR_R_IN2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LF_PIN), (CallbackFunc)&encoder_ticks_l, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RF_PIN), (CallbackFunc)&encoder_ticks_r, RISING);

    speed_controller_l.begin();
    speed_controller_l.tune(SPEED_L_Kp, SPEED_L_Ki, SPEED_L_Kd);
    speed_controller_l.limit(-255, 255);

    speed_controller_r.begin();
    speed_controller_r.tune(SPEED_R_Kp, SPEED_R_Ki, SPEED_R_Kd);
    speed_controller_r.limit(-255, 255);
}

void WheelController::update(){
    uint32_t now = millis();
    uint32_t duration = now < last_seen ? (UINT32_MAX - last_seen + now) : (now - last_seen);
    if(duration < REFRESH_INTERVAL) return;
    int32_t tps_l = (encoder_pos_l - prev_pos_l) / duration;
    int32_t tps_r = (encoder_pos_r - prev_pos_r) / duration;
    // write graph

    if(dist2go_l != 0){
        // update left controller
    }
    if(dist2go_r != 0){
        // update right controller
    }
    // calculate finite speed
    double pwm_l = speed_controller_l.compute(tps_l);
    double pwm_r = speed_controller_r.compute(tps_r);

    // drive motors
    setMotorPwm<MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN>(pwm_l);
    setMotorPwm<MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN>(pwm_r);

    prev_pos_l = encoder_pos_l;
    prev_pos_r = encoder_pos_r;
    last_seen = now;
}

template<uint8_t in1_pin, uint8_t in2_pin>
void WheelController::setMotorPwm(double speed){
    uint8_t pwm = (uint8_t)fabs(speed);
    if(pwm == 0){
        return;
    }
    digitalWrite(pwm < 0 ? in2_pin : in1_pin, 0);
    analogWrite(pwm < 0 ? in1_pin : in2_pin, pwm);
}

void WheelController::move(int32_t dist_l, int32_t dsit_r){

}

void WheelController::moveSpeed(int32_t speed_l, int32_t speed_r){
    speed_controller_l.setpoint(speed_l);
    speed_controller_r.setpoint(speed_r);
    dist2go_l = 0;
    dist2go_r = 0;
}
