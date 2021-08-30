#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

// Marcos
#define clamp(X, _min, _max) (max(_min, min(_max, X)))

typedef void (*CallbackFunc)();

typedef void (*CallbackFunc)();

#pragma region Parameters
/*
    Wheels
*/
#define REFRESH_INTERVAL 100

#define SPEED_L_Kp 1
#define SPEED_L_Ki 0
#define SPEED_L_Kd 0

#define SPEED_R_Kp 1
#define SPEED_R_Ki 0
#define SPEED_R_Kd 0

#define DIST_L_Kp 1
#define DIST_L_Ki 0
#define DIST_L_Kd 0

#define DIST_R_Kp 1
#define DIST_R_Ki 0
#define DIST_R_Kd 0

#define UPDATA_TIME 100

#define __Kp_count_l 260 
#define __Ki_count_l 2.7 
#define __Kd_count_l 2000 

#define __Kp_count_r 260 
#define __Ki_count_r 2.7 
#define __Kd_count_r 2000 

#define __Kp_l 1.88
#define __Ki_l 0.05
#define __Kd_l 2

#define __Kp_r 1.81
#define __Ki_r 0.03
#define __Kd_r 3

/*
    Steppers, EndStops & Claw
*/
#define SCREW_LEAD 8 //mm
#define STEP_ANGLE 1.8
#define STEPS_PER_REV 360 / STEP_ANGLE // 200
#define MICROSTEP 4 // 1/MICROSTEP

#define STEPPER_Y_MAX_SPEED 2500
#define STEPPER_Z_MAX_SPEED 3500

#define STEPPER_MOVE_ACCEL 150

#define ENDSTOP_DEBRONCE_TIME 50

#pragma endregion

#pragma region Pins
/*
    Wheels
*/
#define MOTOR_L_IN1_PIN 9// Left motor IN1
#define MOTOR_L_IN2_PIN 10// Left motor IN2

#define MOTOR_R_IN1_PIN 11// Right motor IN1
#define MOTOR_R_IN2_PIN 12// Right motor IN2

#define ENCODER_RF_PIN 3// Front left encoder
#define ENCODER_RR_PIN A4// Front left encoder

#define ENCODER_LF_PIN 2// Front right encoder
#define ENCODER_LR_PIN A5// Rear right encoder
/*
    Timeout
*/
#define Time_OUT 1

/*
    Steppers, EndStops & Claw
*/
#define STEPPER_Y_STEP_PIN 6
#define STEPPER_Y_DIR_PIN 7

#define STEPPER_Z_STEP_PIN 4
#define STEPPER_Z_DIR_PIN 5

#define ENDSTOP_Y_UPPER_PIN A0
#define ENDSTOP_Y_LOWER_PIN A1

#define ENDSTOP_Z_UPPER_PIN A3
#define ENDSTOP_Z_LOWER_PIN A2

#define CLAW_TRIGGER_PIN 8
#define CLAW_SERVO_PIN 13

#pragma endregion
#endif
