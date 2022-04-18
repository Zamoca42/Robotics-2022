#include <PRIZM.h>

PRIZM prizm;

void setup() {

  prizm.PrizmBegin();

}

void loop() {
  prizm.setMotorPowers(10,10);
  delay(1000);
  prizm.setMotorPowers(125,125);
  delay(1000);
  prizm.setMotorPowers(-10,-10);
  delay(1000);
  prizm.setMotorPowers(125,125);
  delay(1000);
  prizm.setMotorPowers(-20,-20);
  delay(1000);
  prizm.setMotorPowers(-10,-10);
  delay(1000);
}
