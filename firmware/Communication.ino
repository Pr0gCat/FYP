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
  if ((millis() - time) > COM_TIMEOUT && flag != 0)
  {
    flag = 0;
    count = 0;
    checksum = 0;
    data_len = 0;
  }
  digitalWrite(13, 0);
  while (Serial.available())
  {
    if (flag < 2) // Command & length
    {
      buff[count] = Serial.read();
      checksum += buff[count];
      time = millis();
      flag++;
      count++;
      if (flag == 2 && !buff[1])
      {
        flag++;
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
      }else{
        send_msg(String("[Com] Invalid checksum!"));
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
  //TODO: you should check the length of data before using it
  switch (cmd)
  {
  case CMD_INIT:
    send_msg("Start initialization");
    car_init();
    break;

  case CMD_SET_LINEFOLLOW_MODE:
    set_camera_mode(LineFollow);
    confirm(cmd);
    break;

  case CMD_SET_PICKUP_MODE:
    set_camera_mode(PickUp);
    confirm(cmd);
    break;

  case CMD_SET_DROPOFF_MODE:
    set_camera_mode(DropOff);
    confirm(cmd);
    break;

  case CMD_MOVE_ABSY:
    lifter_move_abs(Y, data[0] + (data[1] << 8));
    break;

  case CMD_MOVE_ABSZ:
    lifter_move_abs(Z, data[0] + (data[1] << 8));
    break;

  case CMD_MOVE_RELY:
    lifter_move_rel(Y, data[0] + (data[1] << 8));  
    break;

  case CMD_MOVE_RELZ:
    lifter_move_rel(Z, data[0] + (data[1] << 8));
    break;

  case CMD_STOP_Y:
    lifter_stop_y();
    break;

  case CMD_STOP_Z:
    lifter_stop_z();
    break;

  case CMD_HOME_Y:
    lifter_homeY();
    break;

  case CMD_HOME_Z:
    lifter_homeZ();
    break;

  case CMD_RUN_SPEED:
    run_speed(data[0] + (data[1] << 8),\
     data[2] + (data[3] << 8));
    break;

  case CMD_RUN_DISTANCE:
    run_distance(data[0] + (data[1] << 8), \
    data[2] + (data[3] << 8), 200);
    break;

  default:
    send_msg(String("[Com] Unknown command id found: " + String(cmd)));
  }
}

void confirm(int cmd)
{
  if (cmd != 1)
  {
    Serial.write(CMD_COFIRM);
    Serial.write(1);
    Serial.write(cmd);
    Serial.write((2 + cmd) & 0xff);
  }else{
    send_msg("[Com] Attempt to send confirm to the host");
  }
}

void car_init()
{
  set_camera_mode(DropOff);
  delay(500);
  set_camera_mode(LineFollow);
  delay(500);
  set_camera_mode(PickUp);
  
  calibrateZ();
  calibrateY();
  lifter_homeY();
  lifter_homeZ();
  confirm(CMD_INIT);
}

void send_msg(char msg[])
{
  int str_len = strlen(msg);
  Serial.write(CMD_MSG);
  Serial.write(str_len);
  Serial.write(msg);
  int checksum = 0;
  checksum += CMD_MSG;
  checksum += str_len;
  for (int i = 0; i < str_len; i++)
  {
    checksum += msg[i];
  }
  checksum = (0xff & checksum);
  Serial.write(checksum);
}

void send_msg(String str){
  send_msg(str.c_str());
}