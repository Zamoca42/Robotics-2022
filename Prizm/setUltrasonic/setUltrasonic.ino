#include <PRIZM.h>

PRIZM prizm;

void setup() {

  prizm.PrizmBegin();
  Serial.begin(9600);

}

void loop() {
  Serial.print(prizm.readSonicSensorCM(3));

  Serial.println("CM");
  delay(200);
}
