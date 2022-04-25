#include <PRIZM.h>

PRIZM prizm;

int threshold = 140;
float gain = 0.02;

/***************************************************/
void align();
void rightTurn(int degree); // degree만큼 오른쪽으로 돌기
void leftTurn(int degree);  // degree만큼 왼쪽으로 돌기
void forward(int degree);   // degree만큼 앞으로 가기
void backward(int degree);  // degree만큼 뒤로 가기
void lineTracer();
void backLineTracer();
/***************************************************/

void setup() {
  prizm.PrizmBegin();
  prizm.setMotorInvert(1, 1); //모터를 한 방향으로 전환;
  
  Serial.begin(9600);
  
  // read battery
  Serial.print("Battery");
  Serial.print(prizm.readBatteryVoltage() / 100);
  Serial.println("V");
}



void loop() {
  delay(100);
  Serial.print(prizm.readEncoderDegrees(1));
  Serial.print("왼 - 오");
  Serial.println(prizm.readEncoderDegrees(2));
  while(1)
  {
    lineTracer();
  }
  while(1)
  {
    leftTurn(300);
    rightTurn(300);
    break;
  }
  //if(prizm.readLineSensor(2) == 1)
  //{
    //Serial.println(prizm.readLineSensor(2));
    //align();
    //backward(180);
    //Serial.println("backwardEND");
  //}
  //else
  //{
    //backLineTracer();
    //Serial.println("this is backline");
  //}
  backward(300);
  Serial.println("backwardEND");
  forward(300);
  Serial.println("forwardEND");
}

void align()
{
  prizm.resetEncoder(1);
  prizm.resetEncoder(2);
  prizm.setMotorDegrees(180, 0, 180, 0); // align
}

void forward(int degree)
{
  prizm.setMotorDegrees(180, degree, 180, degree);
  delay((degree / 180) * 2000);
}

void backward(int degree)
{
  prizm.setMotorDegrees(180, -degree, 180, -degree);
  delay((degree / 180) * 2000);
}

void rightTurn(int degree)
{
  align();
  prizm.setMotorDegrees(180, degree, 180, -degree);
  delay((degree / 180) * 2000);
}

void leftTurn(int degree)
{
  align();
  prizm.setMotorDegrees(180, -degree, 180, degree);
  delay((degree / 180) * 2000);
}

void backLineTracer()
{
  int sensor = analogRead(1); // 아날로그 - turn보정에 사용
  float sig = (sensor - threshold) * gain;
  
  Serial.println(sig);
  prizm.setMotorPowers(-15 + sig, -15 + sig);
}
