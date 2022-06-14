#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Twist.h>

#include "SoftwareSerial.h"

#define LED 16

int incoming_data = 0;

#define PIN_TX 12 // D6
#define PIN_RX 14 // D5

SoftwareSerial swSerial(PIN_RX, PIN_TX);

//////////////////////
// WiFi Definitions //
//////////////////////
// WiFi configuration. Replace '***' with your data
const char* ssid     = "drcl-E590"; //"DRCL-WIFI6_2G"; //"drcl-E590";
const char* password = "DRCL1234"; //"DRCLy121";  //"DRCL1234";
IPAddress server(192,168,0,188);      // Set the rosserial socket server IP address
const uint16_t serverPort = 11411;    // Set the rosserial socket server port

// 모터 목표 속도 및 듀티에 관련한 변수 초기화 선언
float R_goal_vel = 0 , L_goal_vel = 0, Ang_goal_vel;
int L_duty = 0, R_duty = 0;
float Length = 0.340;

// ROS encoder data type
geometry_msgs::Vector3 encoder;
ros::Publisher pub_encoder("encoder", &encoder);
// ros::Publisher chatter("chatter", &str_msg);
ros::NodeHandle nh;

// ROS CallBack Function
void speedCmdCallback(const geometry_msgs::Twist& msg) 
{
  Serial.print("odr cmd:");
  Serial.print(msg.linear.x);
  Serial.print(",");
  Serial.println(msg.angular.z/10);

  Ang_goal_vel = msg.angular.z/10;
  
  R_goal_vel = msg.linear.x - (Ang_goal_vel/10);
  L_goal_vel = msg.linear.x + (Ang_goal_vel/10);

  //R_goal_vel = R_goal_vel-(Ang_goal_vel);
  //L_goal_vel = L_goal_vel+(Ang_goal_vel);
    
  swSerial.print(String(R_goal_vel*1250));
  swSerial.print(',');
  swSerial.print(String(L_goal_vel*1250));
  swSerial.print('>');

  delay(5);

  Serial.print("speed cmd:");
  Serial.print(L_goal_vel*1250);
  Serial.print(",");
  Serial.println(R_goal_vel*1250);
}

ros::Subscriber<geometry_msgs::Twist> sub_speedcmd("/robot_odr_cmd", &speedCmdCallback);

void wifisetup()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  nh.initNode();
  nh.advertise(pub_encoder);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  swSerial.begin(9600);
  Serial.println("Software Serial Opened");
  
  wifisetup();
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  pinMode(LED,OUTPUT);
  digitalWrite(LED,1);  

  nh.subscribe(sub_speedcmd);
}

String str = "";

void loop() 
{
  nh.spinOnce();

  if(swSerial.available())
  {
    str = swSerial.readStringUntil('>');

    int first = str.indexOf(",");
    int second = str.indexOf(",",first+1);
    int strlength = str.length();

    String str1 = str.substring(0,first);
    String str2 = str.substring(first+1,strlength);

    encoder.x = str1.toInt();
    encoder.y = str2.toInt();

    //Serial.print(encoder.x);
    //Serial.print(",");
    //Serial.println(encoder.y);
 
    //pub_encoder.publish(&encoder);
    digitalWrite(LED,1);
  }
  delay(10);
  digitalWrite(LED,0);  
}
