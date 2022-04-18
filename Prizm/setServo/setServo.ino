#include <PRIZM.h>

PRIZM prizm;

void setup() {

  prizm.PrizmBegin();
  prizm.setServoSpeed(1,25);

}

void loop() {
  
  prizm.setServoPosition(1,180);
  prizm.setServoPosition(2,180);
  delay(3000);

  prizm.setServoPosition(1,0);
  prizm.setServoPosition(2,0);
  delay(3000);
}
