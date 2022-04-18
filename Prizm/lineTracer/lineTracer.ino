#include <PRIZM.h>
#include "HUSKYLENS.h"

HUSKYLENS huskylens;
PRIZM prizm;

// prizm.readSonicSensorCM(3) // 초음파 센서
// prizm.readLineSensor(2); //디지털센서 0이면 하얀색, 1이면 검은색
int threshold = 150;
float gain = 0.03;
int lineCount = 0;      // 교차로체크
bool isgripID1 = false; // id1에서 그립사용 여부
bool isgripID2 = false; // id2에서 그립사용 여부

/*************************************************/
void suddenstop();
void grip();
void placement();
void liftUp();
void liftDown();
void forward();
void rightTurn();
void leftTurn();
void rotate();
void line();
void backward();
void printResult(HUSKYLENSResult result);
/*************************************************/

void setup()
{
  prizm.PrizmBegin();
  prizm.setMotorInvert(1, 1);

  Wire.begin();

  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin failed!"));
    delay(100);
  }

  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION);

  Serial.begin(9600);
  pinMode(A1, INPUT);

  // set servo
  prizm.setServoSpeed(1, 25); // left
  prizm.setServoSpeed(2, 25); // right
  prizm.setServoSpeed(3, 25); // up&down
  prizm.setServoPosition(1, 80);
  prizm.setServoPosition(2, 110);
  prizm.setServoPosition(3, 35);
}

void loop()
{
  if (!huskylens.request() || !huskylens.isLearned() || !huskylens.available())
  {
    Serial.println(F("Recheck the connection!"));
  }
  else
  {
    HUSKYLENSResult result = huskylens.read();
    // Serial.println(String() + F(",ID=") + result.ID);
    if (result.ID == 1)
    {
      // Serial.println(F("캔"));
      suddenstop();
      placement();
      while (!isgripID1) // id1 그립잡기 실행
      {
        if (prizm.readSonicSensorCM(3) > 4)
        {
          forward();
          delay(500);
        }
        else if (prizm.readSonicSensorCM(3) <= 4)
        {
          forward();
          delay(250);
          suddenstop();
          grip();
          liftUp();
          isgripID1 = true;
        }
      }
    }
    else if (result.ID == 2)
    {
      // Serial.println(F("플리스틱"));
      suddenstop();
      placement();
      while (!isgripID2) // id2 그립잡기 실행
      {
        if (prizm.readSonicSensorCM(3) > 6)
        {
          forward();
          delay(500);
        }
        else if (prizm.readSonicSensorCM(3) <= 6)
        {
          forward();
          delay(250);
          suddenstop();
          grip();
          liftUp();
          isgripID2 = true;
        }
      }
    }
    else
    {
      if (prizm.readLineSensor(2) == 0)
      { //교차로 없으면 직진
        line();
      }
      else if (prizm.readLineSensor(2) == 1)
      {
        suddenstop();
        switch (lineCount) // linecount 추가하면서 교차로추가
        {
        case 0:
          forward();
          delay(700);
          suddenstop();
          lineCount++;
          break;
        case 1:
          forward();
          delay(700);
          rightTurn();
          suddenstop();
          lineCount++;
          break;
        case 2:
          break;
        }
        if (isgripID1 == true && isgripID2 == false)
        {                    // id1이 그립을 쥔상태
          switch (lineCount) // linecount 되돌아가기
          {
          case 0:
            suddenstop();
            break;
          case 1:
            //코너별로 행동
            break;
          case 2:
            break;
          }
          liftDown();
          placement();
          backward();
          delay(1000);
          rotate();
          //라인복귀
          isgripID1 = false;
        }
        else if (isgripID1 == false && isgripID2 == true)
        {
          switch (lineCount) // linecount 빼면서 되돌아가기
          {
          case 0:
            forward();
            delay(2000);
            suddenstop();
            break;
          case 1:
            //코너별로 행동
            lineCount--;
            break;
          case 2:
            break;
          }
          
          liftDown();
          placement();
          backward();
          delay(1000);
          rotate();
          //라인복귀
          isgripID2 = false;
        }
      }
    }
  }
}

void printResult(HUSKYLENSResult result)
{
  if (result.command == COMMAND_RETURN_BLOCK)
  {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
  else if (result.command == COMMAND_RETURN_ARROW)
  {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else
  {
    Serial.println("Object unknown!");
  }
}

void suddenstop()
{
  prizm.setMotorPowers(125, 125);
}

void grip()
{
  prizm.setServoPosition(1, 100);
  prizm.setServoPosition(2, 80);
  delay(3000);
}

void placement()
{
  prizm.setServoPosition(1, 10);
  prizm.setServoPosition(2, 170);
  delay(3000);
}

void liftUp()
{
  prizm.setServoPosition(3, 0);
  delay(3000);
}

void liftDown()
{
  prizm.setServoPosition(3, 95);
  delay(3000);
}

void forward()
{
  prizm.setMotorPowers(20, 20);
}

void rightTurn()
{
  prizm.setMotorPowers(50, -50);
  delay(700); //딜레이가 길수록 많이돈다.
  prizm.setMotorPowers(125, 125);
  delay(1000);
}

void leftTurn()
{
  prizm.setMotorPowers(-50, 50);
  delay(650); //딜레이가 길수록 많이돈다.
  prizm.setMotorPowers(125, 125);
  delay(1000);
}

void rotate()
{
  for (int i = 1; i <= 2; i++)
  {
    rightTurn();
  }
}

void line()
{
  int sensor = analogRead(1); // 아날로그 - turn보정에 사용
  float turn_rate = (sensor - threshold) * gain;

  prizm.setMotorPowers(15 - turn_rate, 15 + turn_rate);
}

void backward()
{
  prizm.setMotorPowers(-20, -20);
}
