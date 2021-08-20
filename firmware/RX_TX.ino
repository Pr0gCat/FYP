#define COFIRM 1
#define GET_CRAWL_STATE 2
#define GET_CRAWL_TRIGGER 3
#define GET_POSY 4
#define GET_POSZ 5
#define GET_SPEED 6
#define LIMIT_TRIGGERED 7

char buff[300];
char flag = 0;
int count = 0;
int checksum = 0;
int data_len = 0;

void cmd_update() // run over and over
{
  while(Serial.available()){
    if(flag == 0 ){
      buff[count] = Serial.read();
      checksum+=buff[count]-'0';
      flag=1;
      count++;
    }
    else if(flag == 1){
      buff[count] = Serial.read();
      checksum+=buff[count]-'0';
      count++;
      flag=2;
    }
    else if(flag == 2){
      int len = (buff[1]-'0');
       buff[count]=Serial.read();
       checksum +=buff[count]-'0';
       count++;
       data_len++;
      if(data_len == len){
        flag=3;
      }
    }
    else if(flag == 3){
      buff[count] = Serial.read();
      int cs = buff[count] - '0';
      count++;
      if((0xff & checksum) == cs){
        unpack();
      }
      count = 0;
      flag = 0;
      checksum = 0;
    }
  }
}

void unpack(){
  int cmd = buff[0]-'0';
  int len = buff[1]-'0';
  char data[255];
  for(int i=0;i < len;i++){
    data[i]=buff[2+i];
  }
  switch (cmd) {
    case COFIRM:
      comfirm(cmd);
      break;
    case GET_CRAWL_STATE:
      get_crawl_state(data);
      break;
    case GET_CRAWL_TRIGGER:
      get_crawl_trigger(data);
      break;
    case GET_POSY:
      get_posy(data);
      break;
    case GET_POSZ:
      get_posz(data);
      break;
    case GET_SPEED:
      get_speed(data);
      break;
    case LIMIT_TRIGGERED:
      limit_trigger(data);
      break;
  }
}

void comfirm(int cmd){
  if(cmd != 1){
    Serial.write(1);
    Serial.write(1);
    Serial.write(cmd);
  }
}

void get_crawl_state(char data[]){
  comfirm(GET_CRAWL_STATE);
}

void get_crawl_trigger(char data[]){
  comfirm(GET_CRAWL_TRIGGER);
}

void get_posy(char data[]){
  comfirm(GET_POSY);
}

void get_posz(char data[]){
  comfirm(GET_POSZ);
}

void get_speed(char data[]){
  comfirm(GET_SPEED);
}

void limit_trigger(char data[]){
  comfirm(LIMIT_TRIGGERED);
}