#include <PRIZM.h>
PRIZM prizm;

void setup() {
 prizm.PrizmBegin();
 prizm.setMotorInvert(1,1);

}

void loop() {
  if(prizm.readLineSensor(2) == 0) {
    prizm.setMotorPowers(125,30);
    prizm.setRedLED(LOW);
  }
  
  if(prizm.readLineSensor(2) == 1) {
    prizm.setMotorPowers(30,125);
    prizm.setRedLED(HIGH);
  }
}
