#include "Adafruit_VL53L0X.h"
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "SK_WiFiGIGA020D"
#define STAPSK  "1603062059"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "15.165.204.254";
const uint16_t port = 8008;

int g_pin = 14; //14번 핀은 D5
int b_pin = 12; //12번 핀은 D6
int r_pin = 13; //13번 핀은 D7

Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // 거리측정 센서 객체 생성

void setup()
{
  Serial.begin(115200);
  pinMode(r_pin, OUTPUT);
  pinMode(g_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);
  // 해당 LED 스트립은 핀에 GND(0v)를 연결해야 점등됩니다.
  // 따라서 VCC(3.3v)에 연결해주어야 LED에 불이 켜지지 않습니다.
  // 펌웨어가 실행되면 불이 꺼진 상태로 펌웨어를 시작하기 위해 아래와 같이 VCC에 연결해줍니다.

  digitalWrite(r_pin, HIGH);
  digitalWrite(g_pin, HIGH);
  digitalWrite(b_pin, HIGH);
// wait until serial port opens for native USB devices
  while (! Serial)
  {
    delay(1);
  }
 
  Serial.println("Adafruit VL53L0X test");
  
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}


WiFiClient client;
char ch;
int i =0;
int user_switch = 0;

void loop()
{

  if(i==0){
    if (client.connect(host, port)){
      i++;
    }
  }
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
   
  if (measure.RangeStatus != 4) // 거리 측정이 정상적으로 이뤄진 경우
  { // phase failures have incorrect data
  Serial.print("Distance (mm): "); 
  Serial.println(measure.RangeMilliMeter);
  }
  else // 거리 측정이 정상적으로 이뤄지지 않은 경우
  {
  Serial.println(" out of range ");
  }
  if(user_switch == 0){
    if(measure.RangeMilliMeter<1000){ // 물체가 300mm 이내에 있을 경우
      digitalWrite(b_pin, LOW); // 빨간색 LED 점등
      delay(10000);
    }
    else{ // 물체가 300mm 이내에 없을 경우
      digitalWrite(r_pin, HIGH);
      digitalWrite(g_pin, HIGH);
      digitalWrite(b_pin, HIGH);
    } 
  }

  else if(user_switch == 1){
    digitalWrite(b_pin, LOW); // 빨간색 LED 점등
  }
  
  if (client.connected()) {
    while(client.available()) {
      //Serial.print(WiFi.status());
      ch = static_cast<char>(client.read());
      Serial.print(ch);
      client.println(ch);

      if(ch == 'a'){
        user_switch = 0;
      }
      
      else if(ch == 'b'){
        user_switch = 1;
      }
      
    } 
    

  }

  else{
    Serial.println("connection error");
    ESP.reset(); 
  }
}
