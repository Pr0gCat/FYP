#include "Communication.h"
#include "WheelController.h"
#include "LifterController.h"
#include "EndEffectorController.h"

#include "config.h"

char buff[300];
char flag = 0;
int count = 0;
uint32_t checksum = 0;
int data_len = 0;
uint32_t time = 0;

void cmd_update() // run over and over
{
  if ((millis() - time) > Time_OUT && flag != 0)
  {
    flag = 0;
    count = 0;
    checksum = 0;
    data_len = 0;
  }
  digitalWrite(13, 0);
  // Serial.println("cmd_update");
  while (Serial.available())
  {
    if (flag < 2) // Command & length
    {
      buff[count] = Serial.read();
      checksum += buff[count];
      time = millis();
      flag++;
      count++;
      if(flag==2){
        int len = buff[1];
        if(len==0){
          flag++;
        }
      }
      
    }
    else if (flag == 2) // receive payload
    {
      int len = buff[1];
      buff[count] = Serial.read();
      checksum += buff[count];
      time = millis();
      count++;
      data_len++;
      if (data_len == len)
      {
        flag = 3;
      }
    }
    else if (flag == 3) // checksum
    {
      buff[count] = Serial.read();
      uint8_t cs = buff[count];
      checksum = (0xff & checksum);
      if (checksum == cs)
      {
        digitalWrite(13, 1);
        unpack();
      }
      count = 0;
      flag = 0;
      checksum = 0;
      data_len = 0;
    }
  }
}

void unpack()
{
  int cmd = buff[0];
  int len = buff[1];
  unsigned char data[255];
  for (int i = 0; i < len; i++)
  {
    data[i] = buff[2 + i];
  }
  switch (cmd)
  {
  case CMD_INIT:
    car_init();
    break;
  case CMD_GET_CLAW_STATE:
    get_crawl_state();
    break;
  case CMD_SET_CLAW_STATE:
    set_crawl_state(data);
    break;
  case CMD_SET_LINEFOLLOW_MODE:
    set_camera_mode(LineFollow);
    break;
  case CMD_SET_PICKUP_MODE:
    set_camera_mode(PickUp);
    break;
  case CMD_SET_DROPOFF_MODE:
    set_camera_mode(DropOff);
    break;
  case CMD_GOTO_POSY:
    goto_posy(data);
    break;
  case CMD_GOTO_POSZ:
    goto_posz(data);
    break;
  case CMD_MOVE_POSY:
    move_posy(data);
    break;
  case CMD_MOVE_POSZ:
    move_posz(data);
    break;
  case CMD_HOME_Y:
    lifter_homeY();
    break;
  case CMD_HOME_Z:
    lifter_homeZ();
    break;
  case CMD_SET_MOTOR_SPEED:
    set_motor_speed(data);
    break;
  case CMD_RUN_DISTANCE:
    run_distance(data);
    break;
  default:
    return;
  }
  confirm(cmd);
}

void confirm(int cmd)
{
  if (cmd != 1)
  {
    Serial.write(CMD_COFIRM);
    Serial.write(1);
    Serial.write(cmd);
  }
}

void car_init()
{
  set_camera_mode(LineFollow);
  calibrateZ();
  calibrateY();
  lifter_homeZ();
  lifter_homeY();
}

void get_crawl_state()
{
}

void set_crawl_state(unsigned char data[])
{
  int16_t left = (data[0] + (data[1] << 8));
  int16_t right = (data[2] + (data[3] << 8));

}

void goto_posy(unsigned char data[])
{
  int16_t pos = (data[0] + (data[1] << 8));
  lifter_move(Y, pos);
}

void goto_posz(unsigned char data[])
{
  int16_t pos = (data[0] + (data[1] << 8));
  lifter_move(Z, pos);
}

void move_posy(unsigned char data[])
{
  int16_t pos = (data[0] + (data[1] << 8));
  lifter_move(Y, pos);
}

void move_posz(unsigned char data[])
{
  int16_t pos = (data[0] + (data[1] << 8));
  lifter_move(Z, pos);
}

void set_motor_speed(unsigned char data[])
{
  int16_t left = (data[0] + (data[1] << 8));
  int16_t right = (data[2] + (data[3] << 8));
  runSpeed(left, right);
}

void run_distance(unsigned char data[])
{
  int16_t left = (data[0] + (data[1] << 8));
  int16_t right = (data[2] + (data[3] << 8));
  Serial.print(left);
  Serial.print(right);
  set_speed_l(200);
  set_speed_r(200);
  set_distance_l(left);
  set_distance_r(right);
}

void send_msg(char msg[],int len)
{
  Serial.write(CMD_MSG);
  Serial.write(len);
  Serial.write(msg);
  int checksum =0;
  checksum+=CMD_MSG;
  checksum+=len;
  for(int i=0;i<len;i++){
    checksum+=msg[i];
  }
  checksum = (0xff & checksum);
  Serial.write(checksum);
}