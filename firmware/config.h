#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

// Marcos
#define clamp(X, _min, _max) max(_min, min(_max, X))

#pragma region Parameters
/*
    Wheels
*/
#define PID_UPDATE_INTERVAL 100

#define MOTOR_L_Kp 1.88
#define MOTOR_L_Ki 0.05
#define MOTOR_L_Kd 2

#define MOTOR_R_Kp 1.81
#define MOTOR_R_Ki 0.03
#define MOTOR_R_Kd 3

/*
    Steppers & EndStops
*/
#define STEPPER_HOMING_SPEED 2400
#define STEPPER_HOMING_ACCEL 50

#define ENDSTOP_DEBRONCE_TIME 50

#define SCREW_GAP 8 //mm/rev

#pragma endregion

#pragma region Pins
/*
    Wheels
*/
#define MOTOR_L_PWM_PIN 8// Left motor IN1
#define MOTOR_L_DIR_PIN // Left motor IN2

#define MOTOR_R_PWM_PIN // Right motor IN1
#define MOTOR_R_DIR_PIN // Right motor IN2

#define ENCODER_LF_PIN // Front left encoder
#define ENCODER_LR_PIN // Front right encoder

#define ENCODER_RF_PIN // Front right encoder
#define ENCODER_RR_PIN // Rear right encoder

/*
    Steppers & EndStops
*/
#define STEPPER_Y_STEP_PIN 6
#define STEPPER_Y_DIR_PIN 7

#define STEPPER_Z_STEP_PIN 4
#define STEPPER_Z_DIR_PIN 5

#define ENDSTOP_Y_UPPER_PIN A0
#define ENDSTOP_Y_LOWER_PIN A1

#define ENDSTOP_Z_UPPER_PIN A2
#define ENDSTOP_Z_LOWER_PIN A3

#pragma endregion
#endif