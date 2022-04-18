#include <PRIZM.h>
PRIZM prizm;
#define RUN 3000

void setup() {
  // put your setup code here, to run once:
  prizm.PrizmBegin();
  prizm.setMotorInvert(1,1);
  Serial.begin(9600);
}

void loop() {
  int rotateNumber = 0;
  if(prizm.readSonicSensorCM(3) < 5 && rotateNumber < 4) {
    rotateNumber++; //벽을 만나면 횟수 증가
    suddenStop();
    right();
  } else if(rotateNumber >= 4) { //벽을 4회만나고나면 실행
    if(prizm.readSonicSensorCM(3) > 150) {//초음파센서로 빈공간인지 확인
      suddenStop();
      forward();
      delay(RUN);
      suddenStop();
      prizm.PrizmEnd(); //loop escape
    } else {
      suddenStop();
      right();
    }
  } else {
    suddenStop();
    forward();
  } 
}

void forward(){
  prizm.setMotorPowers(50,50);
}

void backward(){
   prizm.setMotorPowers(-50,-50);
}

void right(){
   prizm.setMotorPowers(50,-50);
   delay(650);
}

void left(){
   prizm.setMotorPowers(-50,50);
   delay(650);
}

void suddenStop(){
   prizm.setMotorPowers(125,125);
   delay(1000);
}
