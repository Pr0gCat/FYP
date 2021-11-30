#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

// Marcos
#define clamp(X, _min, _max) (max(_min, min(_max, X)))

typedef void (*CallbackFunc)();

typedef void (*CallbackFunc)();

#pragma region Parameters
/*
    Receive Timeout
*/
#define Time_OUT 100

/*
    Wheels
*/
#define REFRESH_INTERVAL 100

// #define SPEED_R_Kp 2.4
// #define SPEED_R_Ki 0.03
// #define SPEED_R_Kd 0

// #define SPEED_L_Kp 3
// #define SPEED_L_Ki 0.05
// #define SPEED_L_Kd 0

#define SPEED_R_Kp 2.4*2
#define SPEED_R_Ki 0.03
#define SPEED_R_Kd 0

#define SPEED_L_Kp 3*2
#define SPEED_L_Ki 0.05
#define SPEED_L_Kd 0

/*
    Steppers, EndStops & Claw
*/
#define SCREW_LEAD 8 //mm
#define STEP_ANGLE 1.8
#define STEPS_PER_REV 360 / STEP_ANGLE // 200
#define MICROSTEP 4 // 1/MICROSTEP

#define STEPPER_Y_MAX_SPEED 5000
#define STEPPER_Z_MAX_SPEED 6000

#define STEPPER_MOVE_ACCEL 300

#define ENDSTOP_DEBRONCE_TIME 80

#pragma endregion
#define MOTOR_L_IN1_PIN 9 // Left motor IN1
#define MOTOR_L_IN2_PIN 10 // Left motor IN2

#define MOTOR_R_IN1_PIN 5 // Right motor IN1
#define MOTOR_R_IN2_PIN 6 // Right motor IN2

#define ENCODER_RF_PIN 0 // Front left encoder
#define ENCODER_LF_PIN 1 // Front right encoder

/*
    Steppers, EndStops & Claw
*/
#define STEPPER_Y_STEP_PIN 12
#define STEPPER_Y_DIR_PIN 11

#define STEPPER_Z_STEP_PIN 8
#define STEPPER_Z_DIR_PIN 7

#define ENDSTOP_Y_UPPER_PIN A0
#define ENDSTOP_Y_LOWER_PIN A1

#define ENDSTOP_Z_UPPER_PIN A2
#define ENDSTOP_Z_LOWER_PIN A3

#define CLAW_SDA_PIN 2
#define CLAW_SCL_PIN 3

#pragma endregion
#endif

/*
    End Effector
*/
#define CLAMP_L_SERVO 0
#define CLAMP_R_SERVO 1
#define CAM_SERVO 2

// angle to distance formula: distance*6/5
// distance to pwm: distance*3+125
#define MAX_DISTANCE 65
#define MIN_DISTANCE 40
#define MAX_ANGLE 325 //angle = 80
#define MIN_ANGLE 245 //angle = 48

// angle calculation formula: angle*5/2+125
#define angle2pwm(angle) (angle*5/2+125)
#define ANGLE_LINE_FOLLOWING angle2pwm(70)
#define ANGLE_PICK_UP 450
#define ANGLE_DROP_OFF angle2pwm(60)
