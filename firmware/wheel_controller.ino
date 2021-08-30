#include<PIDController.h>

volatile long int speed_count_l = 0, speed_count_r = 0;
volatile long int distance_l = 0, distance_r = 0;
int motor_enable_r = 0 ,motor_enable_l = 0;
PIDController pid_speed_l, pid_speed_r;
PIDController pid_distance_l, pid_distance_r;

void encoder_l() {
  if (digitalRead(ENCODER_LR_PIN) == HIGH) {speed_count_l++;distance_l++;}
  else {speed_count_l--;distance_l--;}
}

void encoder_r() {
  if (digitalRead(ENCODER_RR_PIN) == HIGH) {speed_count_r++;distance_r++;}
  else {speed_count_r--;distance_r--;}
}

void Set_motor_pwm(int motor_pwm, int motot, int sw, int power)
{
  if(sw) {
    if(power > 0){
      analogWrite(motor_pwm, power);
      digitalWrite(motot, LOW);
    } else {
      analogWrite(motot, abs(power));
      digitalWrite(motor_pwm, LOW);
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
  pid_speed_l.setpoint( speed_l );
  motor_enable_l = 1;
}

void set_speed_r(int speed_r)
{
  pid_speed_r.setpoint( speed_r );
  motor_enable_r = 1;
}

int motor_controller_l(int enable)
{
  int motor_l_pwm = pid_speed_l.compute(speed_count_l);
  int motor_on_l = pid_distance_l.compute(distance_l);
  speed_count_l = 0;
  
  if( motor_on_l > 0){
    Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, enable, motor_l_pwm );
    return 1;
  } 
  if( motor_on_l < 0) {
    Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, enable, motor_l_pwm * -1);
    return 1;
  }
  if( motor_on_l == 0) {
    Set_motor_pwm(MOTOR_L_IN1_PIN, MOTOR_L_IN2_PIN, 0, 0);
    return 0;
  }
  
}

int motor_controller_r(int enable)
{
  int motor_r_pwm = pid_speed_r.compute(speed_count_r);
  int motor_on_r = pid_distance_r.compute(distance_r);
  speed_count_r = 0;

  if( motor_on_r > 0){
    Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, enable, motor_r_pwm);
    return 1;
  } 
  if( motor_on_r < 0){
    Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, enable, motor_r_pwm * -1);
    return 1;
  } 
  if( motor_on_r == 0) {
    Set_motor_pwm(MOTOR_R_IN1_PIN, MOTOR_R_IN2_PIN, 0, 0);
    return 0;
  }
  
}

void wheel_updata()
{
  static long unsigned int last_time = millis();
  if(millis() - last_time  < UPDATA_TIME) return;
  motor_controller_l(motor_enable_l);
  motor_controller_r(motor_enable_r);
  last_time = millis();  
}

void setup() {Serial.begin(115200); 
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

void loop() {
  // put your main code here, to run repeatedly:
  wheel_updata();
}
