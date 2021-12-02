#include "WheelController.h"

#include "config.h"
#include "Communication.h"
#include <PIDController.h>

volatile long int encoder_l = 0, encoder_r = 0;
volatile long int distance_l = 0, distance_r = 0;
int target_dist_l = 0, target_dist_r = 0;
unsigned char pwm_l = 0, pwm_r = 0;
int dir_l = 1, dir_r = 1;
bool en_l = true, en_r = true;
bool in_distance_mode = false;

PIDController speed_controller_l, speed_controller_r;

void encoder_tick_l()
{
    //called when left encoder ticks
    encoder_l++;
    distance_l++;
}

void encoder_tick_r()
{
    //called when right encoder ticks
    encoder_r++;
    distance_r++;
}

void wheel_setup()
{
    // encoder
    pinMode(ENCODER_LF_PIN, INPUT);
    pinMode(ENCODER_RF_PIN, INPUT);
    // motor
    pinMode(MOTOR_L_IN1_PIN, OUTPUT);
    pinMode(MOTOR_R_IN1_PIN, OUTPUT);
    pinMode(MOTOR_L_IN2_PIN, OUTPUT);
    pinMode(MOTOR_R_IN2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_LF_PIN), encoder_tick_l, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RF_PIN), encoder_tick_r, RISING);

    speed_controller_l.begin();
    speed_controller_l.tune(SPEED_L_Kp, SPEED_L_Ki, SPEED_L_Kd);
    speed_controller_l.limit(-255, 255);

    speed_controller_r.begin();
    speed_controller_r.tune(SPEED_R_Kp, SPEED_R_Ki, SPEED_R_Kd);
    speed_controller_r.limit(-255, 255);
}

void wheel_update()
{
    static long unsigned int last_time = millis();
    // maintaing update interval
    if (millis() - last_time < REFRESH_INTERVAL)
        return;

    update_speed();
    if (in_distance_mode)
    {
        if (update_distance())
        {
            confirm(CMD_RUN_DISTANCE);
            in_distance_mode = false;
        }
    }

    //apply speed
    send_pwm_l();
    send_pwm_r();

    last_time = millis();
}

void run_speed(int speed_l, int speed_r)
{
    en_l = (speed_l != 0);
    dir_l = speed_l > 0 ? 1 : -1;
    speed_controller_l.setpoint(abs(speed_l));

    en_r = (speed_r != 0);
    dir_r = speed_r > 0 ? 1 : -1;
    speed_controller_r.setpoint(abs(speed_r));

    in_distance_mode = false;
}

void run_distance(int dist_l, int dist_r, int speed)
{
    en_l = true;
    en_r = true;
    // reset distance counter
    distance_l = 0;
    distance_r = 0;

    dir_l = dist_l > 0 ? 1 : -1;
    target_dist_l = abs(dist_l);

    dir_r = dist_r > 0 ? 1 : -1;
    target_dist_r = abs(dist_r);

    in_distance_mode = true;
    speed_controller_l.setpoint(abs(speed));
    speed_controller_r.setpoint(abs(speed));
}

void stop_motor_l()
{
    en_l = false;
}
void stop_motor_r()
{
    en_r = false;
}

bool update_distance()
{
    // update left motor
    if (target_dist_l < distance_l)
    {
        distance_l = 0;
        en_l = false;
    }

    // update right motor
    if (target_dist_r < distance_r)
    {
        distance_r = 0;
        en_r = false;
    }

    return !en_l && !en_r;
}

void update_speed()
{
    pwm_l = speed_controller_l.compute(encoder_l);
    pwm_r = speed_controller_r.compute(encoder_r);
    encoder_l = 0;
    encoder_r = 0;
}

void send_pwm_l()
{
    if (!en_l)
    {
        //stop
        digitalWrite(MOTOR_L_IN1_PIN, LOW);
        digitalWrite(MOTOR_L_IN2_PIN, LOW);
        return;
    }
    if (dir_l == 1)
    {
        // backward
        digitalWrite(MOTOR_L_IN2_PIN, LOW);
        analogWrite(MOTOR_L_IN1_PIN, pwm_l);
    }
    else
    {
        // forward
        digitalWrite(MOTOR_L_IN1_PIN, LOW);
        analogWrite(MOTOR_L_IN2_PIN, pwm_l);
    }
}

void send_pwm_r()
{
    if (!en_r)
    {
        //stop
        digitalWrite(MOTOR_R_IN1_PIN, LOW);
        digitalWrite(MOTOR_R_IN2_PIN, LOW);
        return;
    }
    if (dir_r == 1)
    {
        // backward
        digitalWrite(MOTOR_R_IN2_PIN, LOW);
        analogWrite(MOTOR_R_IN1_PIN, pwm_r);
    }
    else
    {
        // forward
        digitalWrite(MOTOR_R_IN1_PIN, LOW);
        analogWrite(MOTOR_R_IN2_PIN, pwm_r);
    }
}