#include "Communication.h"
#include "WheelController.h"

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
    }
    else if (flag == 2) // receive payload
    {
      int len = buff[1];
      buff[count] = Serial.read();
      checksum += buff[count];
      time = millis();
      count++;
      data_len++;
      Serial.print(data_len);
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
  case CMD_COFIRM:
    comfirm(cmd);
    break;
  case CMD_SET_MOTOR_SPEED:
    set_motor_speed(data);
    break;
  }
}

void comfirm(int cmd)
{
  if (cmd != 1)
  {
    Serial.write(1);
    Serial.write(1);
    Serial.write(cmd);
  }
}

void get_crawl_state(unsigned char data[])
{
}

void get_crawl_trigger(unsigned char data[])
{
}

void get_posy(unsigned char data[])
{
}

void get_posz(unsigned char data[])
{
}

void get_motor_speed(unsigned char data[])
{
}

void set_motor_speed(unsigned char data[])
{
  int16_t left = (data[0] + (data[1] << 8));
  int16_t right = (data[2] + (data[3] << 8));
  runSpeed(left, right);
}