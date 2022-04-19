#include <PRIZM.h>
#include "HUSKYLENS.h"

HUSKYLENS huskylens;
PRIZM prizm;

#define CAN_DISTANCE 4    //캔 유효거리
#define BOTTLE_DISTANCE 5 //물병 유효거리

// prizm.readSonicSensorCM(3) // 초음파 센서
// prizm.readLineSensor(2); //디지털센서 0이면 하얀색, 1이면 검은색
int threshold = 150;
float gain = 0.03;
int lineCount = 0;     // 교차로체크
bool isgrip = false;
bool isgripID1 = false; // id1에서 그립사용 여부
bool isgripID2 = false; // id2에서 그립사용 여부
int mapping[10];        //교차로 배열
int sensor = analogRead(1); // 아날로그 - turn보정에 사용
float turn_rate = (sensor - threshold) * gain;

/*************************************************/
void suddenstop(); //급정지
void grip();       //그리퍼로 잡기
void placement();  //그립 펼치기
void liftUp();     //서보로 리프트 들어올리기
void liftDown();   //리프트 내리기
void rightTurn();
void leftTurn();
void rotate();             //오른쪽으로 두번 뒤로 돌기
void line();               //직선 선따라가기
void forward(int mstime);  // mstime만큼 앞으로 가기
void backward(int mstime); // mstime만큼 뒤로 가기
void find_thing(int distance, int id);
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

  //허스키렌즈를 시작하면 Object_Classification 모드로 진입
  // Object_Classification은 width, height로 모터제어 불가
  //인식된 Object가 없을 때의 Object_ID = 0
  // heskylens.writeAlgorithm(modename)으로 모드변경가능
  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION);

  Serial.begin(9600);
  pinMode(A1, INPUT);

  // set servo
  prizm.setServoSpeed(1, 25); // left
  prizm.setServoSpeed(2, 25); // right
  prizm.setServoSpeed(3, 25); // up&down
  prizm.setServoPosition(1, 10);
  prizm.setServoPosition(2, 170);
  prizm.setServoPosition(3, 35);
}

void loop()
{
  //허스키렌즈가 연결되었는지 확인
  //연결 확인이 없다면 허스키렌즈 실행자체가 불가
  if (!huskylens.request() || !huskylens.isLearned() || !huskylens.available())
  {
    Serial.println(F("Recheck the connection!"));
  }
  else
  {
    // result.method로 원하는 데이터를 불러올 수 있음
    // method로 불러올 수 있는 값은 width, height, Xcenter, Ycenter, ID
    HUSKYLENSResult result = huskylens.read();
    // Serial.println(String() + F(",ID=") + result.ID);
    if (result.ID == 1)
    {
      // Serial.println(F("캔"));
      grip_thing(CAN_DISTANCE, 1);
    }
    else if (result.ID == 2)
    {
      // Serial.println(F("플리스틱"));
      grip_thing(BOTTLE_DISTANCE, 2);
    }
    else
    { // objectid = 0일때
      if (prizm.readLineSensor(2) == 1)
      {
        suddenstop();
        delay(2000);
        forward(500);
        suddenstop();
        forward(1450);
        suddenstop();
        delay(1000);
        lineCount++;
        //교차로 시작
        switch (lineCount)
        {
        case 0:
          forward(500);
          suddenstop();
          delay(500);
          forward(1450);
          suddenstop();
          delay(1000);
          if (result.ID == 1 || result.ID == 2)
          {
            break;
          } 
          else
          {
            rightTurn();
            lineCount++;
            break;
          }
        case 1:
          forward(1450);
          suddenstop();
          delay(1000);
          leftTurn();
          if (result.ID == 1 || result.ID == 2)
          {
            break;
          } 
          else
          {
            rightTurn();
            lineCount++;
            break;
          }
        case 2:
          forward(1450);
          delay(1450);
          suddenstop();
          delay(1000);
          leftTurn();
          if (result.ID == 1 || result.ID == 2)
          {
            break;
          } 
          else
          {
            rightTurn();
            lineCount++;
            break;
          }
        }
      } 
      else if(lineCount > 2)
      {
        rotate();
        lineCount = 0;
      }
      else
      {
        line();
      }

      if (isgrip == true && isgripID1 == true)
      { // id1이 그립 상태;
  
        int temp_line = 0;
        
        if (prizm.readLineSensor(2) == 1)
        {
          temp_line = lineCount; //교차로 수 넣기
          
          forward(250);
          if (temp_line >= 0)
          {
            switch (mapping[temp_line]) // 교차로 수 만큼 되돌아가기
            {
            case 0:
              forward(250);
              temp_line--;
              break;
            case 1:
              forward(500);
              leftTurn();
              temp_line--;
              break;
            case 2:
              forward(500);
              rightTurn();
              temp_line--;
              break;
            }
          }
        }
        liftDown();
        placement();
        backward(1000);
        delay(1000);
        rotate();
        //라인복귀
        isgrip = false;
      }
      else if (isgrip == true && isgripID2 == true)
      {
        switch (lineCount) // linecount 빼면서 되돌아가기
        {
        case 0:
          forward(1000);
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
        backward(1000);
        rotate();
        //라인복귀
        isgrip = false;
      }
      else if (isgripID1 == true && isgripID2 == true)
      {
        
      }
    }
  }
}

void grip_thing(int distance, int id)
{
  suddenstop();
  placement();
  while(!isgrip){
    if (prizm.readSonicSensorCM(3) > distance)
    {
      delay(250);
      forward(500);
    }
    else if (prizm.readSonicSensorCM(3) <= distance)
    {
      suddenstop();
      grip();
      liftUp();
      rotate();
      if (id == 1)
      {
        isgrip = true;
        isgripID1 = true;
        break;
      }
      else if (id == 2)
      {
        isgrip = true;
        isgripID2 = true;
        break;
      }
    }
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

void forward(int mstime)
{
  prizm.setMotorPowers(20, 20);
  delay(mstime);
}

void rightTurn()
{
  prizm.setMotorPowers(20, -20);
  delay(1100); //딜레이가 길수록 많이돈다.
  prizm.setMotorPowers(125, 125);
  delay(1000);
}

void leftTurn()
{
  prizm.setMotorPowers(-20, 20);
  delay(1100); //딜레이가 길수록 많이돈다.
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
  prizm.setMotorPowers(20 - turn_rate, 20 + turn_rate);
}

void backward(int mstime)
{
  prizm.setMotorPowers(-20, -20);
  delay(mstime);
}
