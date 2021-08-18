#include <AccelStepper.h>

#define EN_PIN 13
#define DIR_PIN 11
#define STEP_PIN 12

#define UPPER_ENDSTOP_PIN 10
#define LOWER_ENDSTOP_PIN 9

AccelStepper m1(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(9600);
  // while(!Serial.available());
  // put your setup code here, to run once:
  pinMode(UPPER_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(LOWER_ENDSTOP_PIN, INPUT_PULLUP);

  pinMode(EN_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  m1.setEnablePin(EN_PIN);
  m1.setPinsInverted(true, false, true);
  m1.enableOutputs();
  m1.setMaxSpeed(2400);
  m1.setAcceleration(50);

  // 移到底
  m1.setSpeed(-1000);
  while(digitalRead(LOWER_ENDSTOP_PIN)){
    m1.runSpeed();
    if(!digitalRead(LOWER_ENDSTOP_PIN)){delay(50);}
  }

  m1.setSpeed(400);
  while(!digitalRead(LOWER_ENDSTOP_PIN)){
    m1.move(400);
    m1.run();
    if(digitalRead(LOWER_ENDSTOP_PIN)){delay(50);}
  }

  m1.setCurrentPosition(0);
  Serial.println(m1.currentPosition());
  delay(500);
  // probe height
  m1.setSpeed(1000);
  while(digitalRead(UPPER_ENDSTOP_PIN)){
    m1.runSpeed();
    if(!digitalRead(UPPER_ENDSTOP_PIN)){delay(50);}
  }
  delay(500);

  m1.setSpeed(400);
  while(!digitalRead(UPPER_ENDSTOP_PIN)){
    m1.move(-400);
    m1.run();
    if(digitalRead(UPPER_ENDSTOP_PIN)){delay(50);}
  }
  Serial.println(m1.currentPosition());

  m1.setAcceleration(100);
  m1.setSpeed(-1000);
  m1.setMaxSpeed(2400);
  m1.moveTo(0);
  m1.disableOutputs();
}

void loop() {
  // Serial.println(digitalRead(LOWER_ENDSTOP_PIN));
  // if(digitalRead(UPPER_ENDSTOP_PIN)){
  //   m1.moveTo(0);
  // }
  // put your main code here, to run repeatedly:
  // if(m1.distanceToGo() == 0){
  //   m1.stop();
  //   // m1.moveTo(-m1.currentPosition());
  // }
  m1.run();
}
