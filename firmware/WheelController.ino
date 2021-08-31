#include<PIDController.h>
#include "config.h"

volatile long int speed_count_l = 0, speed_count_r = 0;
volatile long int distance_l = 0, distance_r = 0;
int motor_enable_r = 0 ,motor_enable_l = 0;
int motor_direction_l = 1,motor_direction_r = 1;
int motor_mode = 0;
PIDController pid_speed_l, pid_speed_r;
PIDController pid_distance_l, pid_distance_r;

void encoder_l() {
  speed_count_l++;distance_l++;
}

void encoder_r() {
  speed_count_r++;distance_r++;
}

void Set_motor_pwm(int motor_pwm, int motot, int sw, int power, int directionm)
{
  if(sw) {
    if(directionm > 0){
      digitalWrite(motot, LOW);
      analogWrite(motor_pwm, power);
    } else {
      digitalWrite(motor_pwm, LOW);
      analogWrite(motot, power);
    }
  } else {
    analogWrite(motor_pwm, LOW);
    digitalWrite(motot, LOW);
  }
}

void stop_motor_l()
{
  motor_enable_l = 0;
}

void stop_motor_r()
{
  motor_enable_r = 0;
}

void set_origin_l()
{
  distance_l = 0;
}

void set_origin_r()
{
  distance_r = 0;
}

void set_distance_l(int distance_l)
{
  pid_distance_l.setpoint( distance_l );
}

void set_distance_r(int distance_r)
{
  pid_distance_r.setpoint( distance_r );
}

void set_speed_l(int speed_l)
{
  if(speed_l > 0)
  {
    motor_direction_l = 1;
    pid_speed_l.setpoint( speed_l );
  } else {
    motor_direction_l = -1;
    pid_speed_l.setpoint( abs(speed_l) );
  }
  motor_enable_l = 1;
}

void set_speed_r(int speed_r)
{
  if(speed_r > 0)
  {
    motor_direction_r = 1;
    pid_speed_r.setpoint( speed_r );
  } else {
    motor_direction_r = -1;
    pid_speed_r.setpoint( abs(speed_r) );
  }
  motor_enable_r = 1;
}

int motor_controller_l(int enable)
{
  int motor_l_pwm = pid_speed_l.compute(speed_count_l);
  Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, enable, motor_l_pwm , motor_direction_l );
  speed_count_l = 0;
}

int motor_controller_r(int enable)
{
  int motor_r_pwm = pid_speed_r.compute(speed_count_r);
  speed_count_r = 0;
  Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, enable, motor_r_pwm , motor_direction_r );
}

int motor_controller_distance_l(int enable)
{
  int motor_l_pwm = pid_speed_l.compute(speed_count_l);
  int motor_on_l = pid_distance_l.compute(distance_l);
  speed_count_l = 0;
  
  if( motor_on_l > 0){
    Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, enable, motor_l_pwm , motor_direction_l );
    return 1;
  } else{
    Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, 0, 0,0);
    return 0;
  }
  
}

int motor_controller_distance_r(int enable)
{
  int motor_r_pwm = pid_speed_r.compute(speed_count_r);
  int motor_on_r = pid_distance_r.compute(distance_r);
  speed_count_r = 0;

  if( motor_on_r > 0){
    Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, enable, motor_r_pwm , motor_direction_r );
    return 1;
  } else {
    Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, 0, 0,0);
    return 0;
  }
  
}

void set_speed_r(int32_t speed)
{
  if(speed > 0)
  {
    motor_direction_r = 1;
    pid_speed_r.setpoint( speed );
  } else {
    motor_direction_r = -1;
    pid_speed_r.setpoint( abs(speed) );
  }
  if(speed == 0) motor_enable_r = 0;
  else motor_enable_r = 1;
}

void set_speed_l(int32_t speed)
{
  if(speed > 0)
  {
    motor_direction_l = 1;
    pid_speed_l.setpoint( speed );
  } else {
    motor_direction_l = -1;
    pid_speed_l.setpoint( abs(speed) );
  }
  if(speed == 0) motor_enable_l = 0;
  else motor_enable_l = 1;
}

void runSpeed(int32_t speed_l, int32_t speed_r)
{
  motor_mode = 0;
  set_speed_r(speed_r);
  set_speed_l(speed_l);
  
}

void runDistanceR(uint32_t distance, int32_t speed)
{
  motor_mode = 1;
  set_speed_r(speed);
}

void runDistanceL(uint32_t distance, int32_t speed)
{
  motor_mode = 1;
  set_speed_l(speed);
}

void wheel_update()
{
  static long unsigned int last_time = millis();
  if(millis() - last_time  < UPDATA_TIME) return;
  if(motor_mode)
  {
    motor_controller_l(motor_enable_l);
    motor_controller_r(motor_enable_r);
  } else {
    motor_controller_distance_l(motor_enable_l);
    motor_controller_distance_r(motor_enable_r);
  }
  last_time = millis();  
}

void wheel_setup() {
  // encoder
  pinMode(ENCODER_LF_PIN, INPUT); 
  pinMode(ENCODER_LR_PIN, INPUT); 
  pinMode(ENCODER_RF_PIN, INPUT); 
  pinMode(ENCODER_RR_PIN, INPUT); 
  // motor
  pinMode(MOTOR_L_IN1_PIN, OUTPUT); 
  pinMode(MOTOR_R_IN1_PIN, OUTPUT); 
  pinMode(MOTOR_L_IN2_PIN, OUTPUT); 
  pinMode(MOTOR_R_IN2_PIN, OUTPUT); 
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_LF_PIN), encoder_l, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RF_PIN), encoder_r, RISING);
  
  pid_speed_l.begin(); 
  pid_speed_l.tune(__Kp_l , __Ki_l , __Kd_l); 
  pid_speed_l.limit(-255, 255); 
  
  pid_speed_r.begin(); 
  pid_speed_r.tune(__Kp_r , __Ki_r , __Kd_r); 
  pid_speed_r.limit(-255, 255); 
  
  pid_distance_l.begin(); 
  pid_distance_l.tune(__Kp_count_l , __Ki_count_l , __Kd_count_l); 
  pid_distance_l.limit(-255, 255); 
  
  pid_distance_r.begin(); 
  pid_distance_r.tune(__Kp_count_r , __Ki_count_r , __Kd_count_r); 
  pid_distance_r.limit(-255, 255); 

}
