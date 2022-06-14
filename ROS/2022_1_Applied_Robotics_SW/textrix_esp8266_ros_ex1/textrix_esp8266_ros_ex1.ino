#include <PRIZM.h>
#include <SoftwareSerial.h>

PRIZM prizm;

#define PIN_RX 2
#define PIN_TX 9

float speed_cmd_left = 0.;
float speed_cmd_right = 0.;

SoftwareSerial swSerial(PIN_RX, PIN_TX);


void setup() {
  Serial.begin(115200);
  prizm.PrizmBegin();
  prizm.setMotorInvert(1, 1);

  swSerial.begin(57600);
}

String str = "";

void loop()
{
  // prizm.resetEncoder(1);
  //prizm.resetEncoder(2);
  int enco1 = prizm.readEncoderCount(1);
  int enco2 = prizm.readEncoderCount(2);

  //Serial.print("enc1=");
  //Serial.print(enco1);
  //Serial.print(", enc2=");
  //Serial.println(enco2);

  //swSerial.print(String(enco1));
  //swSerial.print(',');
  //swSerial.print(String(enco2));
  //swSerial.print('>');

  if(swSerial.available())
  {
    str = swSerial.readStringUntil('>');

    int first = str.indexOf(",");
    int second = str.indexOf(",",first+1);
    int strlength = str.length();

    String str1 = str.substring(0,first);
    String str2 = str.substring(first+1,strlength);

    speed_cmd_left = str1.toFloat();
    speed_cmd_right = str2.toFloat();

    Serial.print(speed_cmd_left);
    Serial.print(",");
    Serial.println(speed_cmd_right);

    prizm.setMotorSpeed(1,speed_cmd_left);
    prizm.setMotorSpeed(2,speed_cmd_right);
  }

  delay(10);
}
