#include "Communication.h"
#include "config.h"

char buff[300];
char flag = 0;
int count = 0;
int checksum = 0;
int data_len = 0;
uint32_t time = 0;

void cmd_update() // run over and over
{
  if ((millis() - time) > Time_OUT && flag!=0)
  {
    flag = 0;
    count = 0;
    checksum = 0;
  }
  while (Serial.available())
  {
    if (flag == 0)
    {
      buff[count] = Serial.read();
      checksum += buff[count] - '0';
      time = millis();
      flag = 1;
      count++;
    }
    else if (flag == 1)
    {
      buff[count] = Serial.read();
      checksum += buff[count] - '0';
      time = millis();
      count++;
      flag = 2;
    }
    else if (flag == 2)
    {
      int len = (buff[1] - '0');
      buff[count] = Serial.read();
      checksum += buff[count] - '0';
      time = millis();
      count++;
      data_len++;
      if (data_len == len)
      {
        flag = 3;
      }
    }
    else if (flag == 3)
    {
      buff[count] = Serial.read();
      int cs = buff[count] - '0';
      count++;
      if ((0xff & checksum) == cs)
      {
        unpack();
      }
      count = 0;
      flag = 0;
      checksum = 0;
    }
  }
}

void unpack()
{
  int cmd = buff[0] - '0';
  int len = buff[1] - '0';
  char data[255];
  for (int i = 0; i < len; i++)
  {
    data[i] = buff[2 + i];
  }
  switch (cmd)
  {
  case CMD_COFIRM:
    comfirm(cmd);
    break;
  case CMD_GET_CRAWL_STATE:
    get_crawl_state(data);
    break;
  case CMD_GET_CRAWL_TRIGGER:
    get_crawl_trigger(data);
    break;
  case CMD_GET_POSY:
    get_posy(data);
    break;
  case CMD_GET_POSZ:
    get_posz(data);
    break;
  case CMD_GET_SPEED:
    get_speed(data);
    break;
  case CMD_LIMIT_TRIGGERED:
    limit_trigger(data);
    break;
  case CMD_SET_SPEED:
    set_speed(data);
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

void get_crawl_state(char data[])
{
}

void get_crawl_trigger(char data[])
{
}

void get_posy(char data[])
{
}

void get_posz(char data[])
{
}

void get_speed(char data[])
{
}

void limit_trigger(char data[])
{
  comfirm(CMD_LIMIT_TRIGGERED);
}

void set_speed(char data[])
{
  uint16_t left =(data[0] -'0') + (data[1] -'0');
  uint16_t right =(data[2] -'0') + (data[3] -'0');
}
