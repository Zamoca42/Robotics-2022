/* This example demonstrates how to read the load current of a DC motor channel and print it to the Serial Moniitor.
 * For more detailed information on using the PRIZM library functions,
 * Please see the Arduino Library functions grid in the TETRIX PRIZM Programmer's guide 
 * which can be viewed and downloaded at www.TETRIXRobotics.com
 */

#include <PRIZM.h>    // Include PRIZM Library

PRIZM prizm;          // Instantiate an object named prizm

void setup() {

  prizm.PrizmBegin();
  prizm.setMotorInvert(1,1);
}


void loop() {

  for(int x=0; x<=3; x++) {
    forward();
    rightTurn();
  }
  prizm.PrizmEnd();
}

void forward() {
  prizm.setMotorPowers(50,50);
  delay(4250); //딜레이가 길수록 많이간다.
  prizm.setMotorPowers(125,125);
  delay(1000);
}

void rightTurn(){
  prizm.setMotorPowers(50,-50);
  delay(650); //딜레이가 길수록 많이돈다.
  prizm.setMotorPowers(125,125);
  delay(1000);
}
