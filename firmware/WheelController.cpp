#include "config.h"
#include <PIDController.h>

volatile uint32_t encoder_l_count = 0, encoder_r_count = 0;
PIDController motor_l_controller, motor_r_controller;
uint32_t last_seen = 0, \
left_last_seen = 0, right_last_seen = 0, \
left_dt = 0, right_dt = 0;

void encoder_l_ticks(){
    uint32_t now = micros();
    // overflowed
    if(now < left_last_seen){
        left_dt = UINT32_MAX - left_last_seen + now;
    }else{
        left_dt = now - left_last_seen;
    }
    left_last_seen = now;
    encoder_l_count++;
}

void encoder_r_ticks(){
    uint32_t now = micros();
    // overflowed
    if(now < right_last_seen){
        right_dt = UINT32_MAX - right_last_seen + now;
    }else{
        right_dt = now - right_last_seen;
    }
    right_last_seen = now;
    encoder_r_count++;
}

void wheels_begin(){
    pinMode(ENCODER_LF_PIN, INPUT);
    pinMode(ENCODER_LR_PIN, INPUT);
    pinMode(ENCODER_RF_PIN, INPUT);
    pinMode(ENCODER_RR_PIN, INPUT);

    pinMode(MOTOR_L_PWM_PIN, OUTPUT);
    pinMode(MOTOR_L_DIR_PIN, OUTPUT);
    pinMode(MOTOR_R_PWM_PIN, OUTPUT);
    pinMode(MOTOR_R_DIR_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LF_PIN), encoder_l_ticks, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RF_PIN), encoder_r_ticks, RISING);

    motor_l_controller.begin();
    motor_l_controller.tune(MOTOR_L_Kp, MOTOR_L_Ki, MOTOR_L_Kd);
    motor_l_controller.limit(0, 255);

    motor_r_controller.begin();
    motor_r_controller.tune(MOTOR_R_Kp, MOTOR_R_Ki, MOTOR_R_Kd);
    motor_r_controller.limit(0, 255);

}

void run(){
    if(millis() - last_seen >= PID_UPDATE_INTERVAL){
        uint8_t motor_l_pwm = motor_l_controller.compute(left_dt);
        uint8_t motor_r_pwm = motor_r_controller.compute(right_dt);
    }
}