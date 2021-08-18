
void setup()
{
  #define COFIRM 1
  #define GET_CRAWL_STATE 2
  #define GET_CRAWL_TRIGGER 3
  #define GET_POSY 4
  #define GET_POSZ 5
  #define GET_SPEED 6
  #define LIMIT_TRIGGERED 7
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Hello");
}

char buff[300];
char flag = 0;
int count = 0;
int checksum = 0;

void loop() // run over and over
{
  while(Serial.available()){
    Serial.write(flag);
    if(flag == 0 ){
      buff[count] = Serial.read();
      checksum+=buff[count];
      flag=1;
      count++;
    }
    else if(flag == 1){
      buff[count] = Serial.read();
      checksum+=buff[count];
      count++;
      flag=2;
    }
    else if(flag == 2){
      for(int i=0 ; i <  Serial.read(); i++){
        checksum +=buff[count];
        count++;
      }
      flag=3;
    }
    else if(flag == 3){
      char cs = Serial.read();
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
  char cmd;
  char len;
  char data[255];
  for(int i=0;i < len;i++){
    data[i]=buff[2+i];
  }
  switch (cmd) {
    case COFIRM:
      comfirm();
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

void comfirm(){

}

void get_crawl_state(char data[]){
    
}

void get_crawl_trigger(char data[]){
  
}

void get_posy(char data[]){
  
}

void get_posz(char data[]){
  
}

void get_speed(char data[]){
  
}

void limit_trigger(char data[]){
  
}
