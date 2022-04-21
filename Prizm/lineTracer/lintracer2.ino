#include <Thread.h>
#include <ThreadController.h>

#include <PRIZM.h>
#include "HUSKYLENS.h"

#define CAN_DISTANCE 4    //캔 거리
#define BOTTLE_DISTANCE 5 //물병 거리

HUSKYLENS huskylens;
PRIZM prizm;
ThreadController controll = ThreadController();


//My simple Thread
Thread myThread = Thread();
Thread myThread2 = Thread();

// prizm.readSonicSensorCM(3) // 초음파 센서
// prizm.readLineSensor(2); //디지털센서 0이면 하얀색, 1이면 검은색
int threshold = 150;
float gain = 0.03;

bool isgrip = false;
bool isgripID1 = false; // objectid1에서 그립사용 여부
bool isgripID2 = false; // objectid2에서 그립사용 여부

int horizonLine = 0; // 가로선체크
int verticalLine = 0; // 세로선체크

/*************************************************/
void suddenstop(); //급정지
void grab();       //그리퍼로 잡기
void placement();  //그립 펼치기
void liftUp();     //서보로 리프트 들어올리기
void liftDown();   //리프트 내리기
void rightTurn(int rad); //rad만큼 오른쪽으로 돌기
void leftTurn(int rad);  //rad만큼 왼쪽으로 돌기
void forward(int mstime);  // mstime만큼 앞으로 가기
void backward(int mstime); // mstime만큼 뒤로 가기
void find_thing(int distance, int id);
void lineTracer();
/*************************************************/

// callback for myThread
void lineFinder()
{ //phase = 0;
  
  if(prizm.readLineSensor(2) == 0) 
  {
    lineTracer();
  }
  else //교차로 만났을 때
  {
    forward(1450);
    suddenstop();
    rightTurn(90);
    delay(500);
    horizonLine++;
    leftTurn(90);
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
    if (result.ID == 1)
    {
      // Serial.println(F("캔"));
      find_thing(CAN_DISTANCE, 1);
    }
    else if (result.ID == 2)
    {
      // Serial.println(F("플리스틱"));
      find_thing(BOTTLE_DISTANCE, 2);
    }
    else
    { 
      // objectid = 0일 때
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
  pinMode(A1, INPUT); //TCRT-5000

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

  //set thread
  myThread.onRun(lineFinder);
  myThread.setInterval(20);
  
  myThread2.onRun(objectClassify);
  myThread2.setInterval(100);
  
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
  controll.run();

  // Other code...
}

void find_thing(int distance, int id)
{
  suddenstop();
  placement();
  while (!isgrip)
  {
    if (prizm.readSonicSensorCM(3) > distance)
    {
      lineTracer();
      delay(15);
      if(prizm.readLineSensor(2) == 1)
      {
        forward(100);//선 뛰어넘기
        verticalLine++;
      }
    }
    else if (prizm.readSonicSensorCM(3) <= distance)
    {
      suddenstop();
      grab();
      liftUp();
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

void grab()
{
  prizm.setServoPosition(1, 100);
  prizm.setServoPosition(2, 80);
}

void placement()
{
  prizm.setServoPosition(1, 10);
  prizm.setServoPosition(2, 170);
}

void liftUp()
{
  prizm.setServoPosition(3, 0);
}

void liftDown()
{
  prizm.setServoPosition(3, 95);
}

void forward(int mstime)
{
  prizm.setMotorPowers(20, 20);
  delay(mstime);
}

void backward(int mstime)
{
  prizm.setMotorPowers(-20, -20);
  delay(mstime);
}

void rightTurn(int rad)
{
  for(int i = 0; i <= rad; i++)
  {
     prizm.setMotorTargets(0,1,0,-1);
     delay(5);
  }
}

void lineTracer()
{
  int sensor = analogRead(1); // 아날로그 - turn보정에 사용
  float sig = (sensor - threshold) * gain;

  prizm.setMotorPowers(20 - sig,20 + sig);
}

void leftTurn(int rad)
{
  for(int i = 0; i <= rad; i++)
  {
     prizm.setMotorTargets(0,-1,0,1);
     delay(5);
  }
}
