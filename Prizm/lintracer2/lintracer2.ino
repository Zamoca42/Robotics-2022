#include <Thread.h>
#include <ThreadController.h>

#include <PRIZM.h>
#include "HUSKYLENS.h"

// Distance
#define CAN_DISTANCE 4    // 캔 거리
#define BOTTLE_DISTANCE 5 // 물병 거리
// Degree
#define TURN 270  // 90도 도는 인코더 각도
#define WHEEL 270 // 바퀴가 선과 만나는 인코더각도
// Encoder
#define LEFT 1  // LEFT Encoder
#define RIGHT 2 // RIGHT Encoder
// LineSensor
#define WHITE 0
#define BLACK 1

HUSKYLENS huskylens;
PRIZM prizm;
ThreadController controll = ThreadController();

// My simple Thread
Thread myThread = Thread();
Thread myThread2 = Thread();

// prizm.readSonicSensorCM(3) // 초음파 센서
// prizm.readLineSensor(2); //디지털센서 0이면 하얀색, 1이면 검은색
int threshold = 150;
float gain = 0.03;

int gripCount = 0;
bool isgripID1 = false; // objectid1에서 그립사용 여부
bool isgripID2 = false; // objectid2에서 그립사용 여부

int horizonLine = 0; // 가로선체크
int turnCount = 0;   // 턴 횟수
long rightEncoderDegree = 0;
long leftEncoderDegree = 0;

/*************************************************/
void suddenstop();          //급정지
void grab();                //그리퍼로 잡기
void placement();           //그립 펼치기
void liftUp();              //서보로 리프트 들어올리기
void liftDown();            //리프트 내리기
void rightTurn(int degree); // degree만큼 오른쪽으로 돌기
void leftTurn(int degree);  // degree만큼 왼쪽으로 돌기
void forward(int degree);   // degree만큼 앞으로 가기
void backward(int degree);  // degree만큼 뒤로 가기
void find_thing(int distance, int id);
void lineTracer();
void align(); //인코더 0도 정렬
/*************************************************/

// callback for myThread
void lineFinder()
{ // phase = 0;

  if (prizm.readLineSensor(2) == 0)
  {
    lineTracer();
  }
  else //교차로 만났을 때
  {
    while (1)
    {
      suddenstop();
      delay(100);
      align();
      forward(WHEEL);
      suddenstop();
      rightTurn(TURN);
      turnCount++;
      horizonLine++;
      Serial.println(horizonLine);
      Serial.print(prizm.readSonicSensorCM(3));
      Serial.println("CM");
      if (prizm.readSonicSensorCM(3) < 20) //만약 초음파 범위(선길이)내에 물건이 있다면
      {                                    //가까이가기
        align();                           // resetEncoder
        lineTracer();
        forward(540); //허스키렌즈가 물건을 인식하는 거리까지 다가가기
        //엔코더값 저장하기
        rightEncoderDegree = prizm.readEncoderDegrees(2);
        leftEncoderDegree = prizm.readEncoderDegrees(1);
        break;
      }
      else // 20CM 이상이면
      {
        leftTurn(TURN);
        turnCount--;
        break;
      }
    }
  }
}

void objectClassify()
{
  //허스키렌즈가 연결되었는지 확인
  //연결 확인이 없다면 허스키렌즈 실행자체가 불가
  if (!huskylens.request() || !huskylens.isLearned() || !huskylens.available())
  {
    Serial.println(F("Recheck the connection!"));
  }
  else
  {
    HUSKYLENSResult result = huskylens.read();
    delay(100);
    if (result.ID == 1)
    {
      Serial.println("캔");
      find_thing(CAN_DISTANCE, 1);
    }
    else if (result.ID == 2)
    {
      Serial.println("플라스틱");
      find_thing(BOTTLE_DISTANCE, 2);
    }
  }
}

void setup()
{
  prizm.PrizmBegin();
  prizm.setMotorInvert(1, 1); //모터를 한 방향으로 전환;

  Wire.begin();

  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin failed!"));
    delay(100);
  }

  Serial.begin(9600);
  pinMode(A1, INPUT); // TCRT-5000

  //허스키렌즈를 시작하면 Object_Classification 모드로 진입
  // Object_Classification은 width, height로 모터제어 불가
  //인식된 Object가 없을 때의 Object_ID = 0
  // heskylens.writeAlgorithm(modename)으로 모드변경가능
  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION);

  // set servo
  prizm.setServoSpeed(1, 25); // left
  prizm.setServoSpeed(2, 25); // right
  prizm.setServoSpeed(3, 25); // up&down
  prizm.setServoPosition(1, 10);
  prizm.setServoPosition(2, 170);
  prizm.setServoPosition(3, 35);

  // read battery
  Serail.print(Battery);
  Serial.print(prizm.readBatteryVoltage());
  Serial.println("V");

  // set thread
  myThread.onRun(lineFinder);
  myThread.setInterval(10);

  myThread2.onRun(objectClassify);
  myThread2.setInterval(50);

  controll.add(&myThread);
  controll.add(&myThread2);
}

void loop()
{
  // checks if thread should run
  /*if(myThread.shouldRun())
    myThread.run();

  if(myThread2.shouldRun())
    myThread2.run();*/
  // if(!isgripID1 && !isgripID2)
  controll.run();

  // Other code...
  // after grab
  if (isgripID1)
  {
    //왼쪽, 오른쪽, 센서값 비교
    // findthing에서 인코더값만큼 받아서
    //뒤로 온다
    backward(rightEncoderDegree);
    if (turnCount == 1)
    {
      if (prizm.readLineSensor(2) == 1) //교차로가 맞는지 확인
      {
        forward(WHEEL);
        leftTurn(TURN);
        turnCount--;
      }
      else
      {
        backLineTracer();
        // while(1)
        // {
        //
        //   if(prizm.readLineSensor(2) == 1)
        //   {
        //     suddenstop();
        //     break;
        //   }
        // }
      }
    }
    else if (turnCount == 0) // baseline 상태
    {
      while (horizonLine > 1)
      {
        if (prizm.readLineSensor(2) == 1)
        {
          horizonLine--;
          suddenstop();
          backward(45);
        }
        else
        {
          backLineTracer();
        }
      }
      // Linecount 교차로수 만큼 뒤로와서
      // 1번위치 놓기
      //물건놓기
      //처음위치로
    }
    if (horizonLine == 1)
    {
      int store = 0; //놓을 위치 id에 따라 다름
      forward(WHEEL);
      rightTurn(TURN);
      //자리찾기
      while (1)
      {
        if (prizm.readLineSensor(2) == 1)
        {
          forward(WHEEL);
          store++;
          if (store == 1) // id값
          {
            rightTurn(TURN);
            forward(WHEEL * 2);
            placement();
            backward(WHEEL * 2);
            break;
          }
        }
        else
        {
          backLineTracer();
        }
      }
      //되돌아나오기
      while (1)
      {
        if (prizm.readLineSensor(2) == 1)
        {
          if (store == 1) // id값
          {
            forward(WHEEL);
            leftTurn(TURN);
            store--;
            break;
          }
        }
        else
        {
          backLineTracer();
        }
      }

      if (prizm.readLineSensor(2) == 1)
      {
        forward(WHEEL);
        leftTurn(TURN);
        isgripID1 = false;
        gripCount++;
      }
      else
      {
        backLineTracer();
      }
    }
  }
  else if (isgripID2)
  {
    prizm.PrizmEnd();
  }
  // if(gripCount == 2)
  // {
  //   prizm.PrizmEnd();
  // }
}

void find_thing(int distance, int id)
{
  prizm.resetEncoder(1); //여기서부터 왼쪽 엔코더값 측정
  prizm.resetEncoder(2); // 여기서부터 오른쪽 엔코더값 측정

  Serial.print(prizm.readEncoderDegrees(2));   // -> 0
  Serial.println(prizm.readEncoderDegrees(1)); // -> 0

  while (1)
  {
    if (prizm.readSonicSensorCM(3) > distance)
    {
      lineTracer();
    }
    else if (prizm.readSonicSensorCM(3) <= distance)
    {
      forward(45);
      suddenstop();
      grab();
      liftUp();
      rightEncoderDegree = rightEncoderDegree + prizm.readEncoderDegrees(2);
      leftEncoderDegree = leftEncoderDegree + prizm.readEncoderDegrees(1);
      Serial.print(leftEncoderDegree);
      Serial.print("왼");
      Serial.print(rightEncoderDegree);
      Serial.println("오");
      if (id == 1)
      {
        isgripID1 = true;
        Serial.println("ENDID1");
        break;
      }
      else if (id == 2)
      {
        isgripID2 = true;
        Serial.println("ENDID2");
        break;
      }
    }
  }
}

void suddenstop()
{
  prizm.setMotorPowers(125, 125);
}

void grab()
{
  prizm.setServoPosition(1, 100);
  prizm.setServoPosition(2, 80);
  delay(2000);
}

void placement()
{
  prizm.setServoPosition(1, 10);
  prizm.setServoPosition(2, 170);
  delay(2000);
}

void liftUp()
{
  prizm.setServoPosition(3, 0);
  delay(2000);
}

void liftDown()
{
  prizm.setServoPosition(3, 95);
  delay(2000);
}

void forward(int degree)
{
  prizm.setMotorDegrees(180, degree, 180, degree);
  delay((degree / 180) * 2000);
}

void backward(int degree)
{
  prizm.setMotorDegrees(180, degree, 180, degree);
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

void align()
{
  prizm.resetEncoder(1);
  prizm.resetEncoder(2);
  prizm.setMotorDegrees(180, 0, 180, 0); // align
  delay(10);
}

void lineTracer()
{
  int sensor = analogRead(1); // 아날로그 - turn보정에 사용
  float sig = (sensor - threshold) * gain;

  prizm.setMotorPowers(20 - sig, 20 + sig);
}

void backLineTracer()
{
  int sensor = analogRead(1); // 아날로그 - turn보정에 사용
  float sig = (sensor - threshold) * gain;

  prizm.setMotorPowers(-20 - sig, -20 + sig);
}

/* TEST함수
void rightTurnTEST(int degree)
{
  align();
  Serial.print(prizm.readEncoderDegrees(1));
  Serial.print(F("왼"));
  Serial.print(prizm.readEncoderDegrees(2));
  Serial.println(F("오"));
  prizm.setMotorDegrees(180, degree, 180, -degree);
  delay((degree / 180) * 2000);
  Serial.print(prizm.readEncoderDegrees(1));
  Serial.print(F("왼"));
  Serial.print(prizm.readEncoderDegrees(2));
  Serial.println(F("오"));
  Serial.println(F("rightTurnEnd"));
}

void lineTEST(int degree)
{
  align();
  Serial.print(prizm.readEncoderDegrees(1));
  Serial.print(F("왼"));
  Serial.print(prizm.readEncoderDegrees(2));
  Serial.println(F("오"));
  prizm.setMotorDegrees(180, -degree, 180, -degree); // backward
  delay((degree / 180) * 2000);
  Serial.print(prizm.readEncoderDegrees(1));
  Serial.print(F("왼"));
  Serial.print(prizm.readEncoderDegrees(2));
  Serial.println(F("오"));
  Serial.println(F("End"));
}
*/