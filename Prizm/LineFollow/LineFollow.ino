#include <PRIZM.h>
PRIZM prizm;

int threshold = 155;
float gain = 0.03;

void setup() {
  prizm.PrizmBegin();
  prizm.setMotorInvert(1,1);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  

}

void loop() {

  int sensor = analogRead(1);
  float turn_rate = (sensor-threshold)*gain;
  int x = 0;

  Serial.println(prizm.readLineSensor(2));
  Serial.println(turn_rate);
  delay(50);

    //if(prizm.readLineSensor(2) == 0)
      //prizm.setMotorPowers(15-turn_rate, 15+turn_rate);
    //else
      //prizm.setMotorPowers(125, 125);
      //검은색이면 멈
}
