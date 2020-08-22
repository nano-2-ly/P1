#include "Adafruit_VL53L0X.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "SK_WiFiGIGA020D"
#define STAPSK  "1603062059"
#endif

char uuid[100] = "1abf4f38-b7c0-41a2-8a7c-221c4adb5740";

const char* ssid     = STASSID;
const char* password = STAPSK;

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

  Serial.println("Adafruit VL53L0X test");
  
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected");
}



WiFiClient client;

int user_switch = 0;
int distance = 9999;
char *comm = "C001";
void loop() 
{
  char apiURL[100];

  VL53L0X_RangingMeasurementData_t measure;
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
   
  if (measure.RangeStatus != 4) // 거리 측정이 정상적으로 이뤄진 경우
  { // phase failures have incorrect data
    Serial.print("Distance (mm): "); 
    distance = measure.RangeMilliMeter;
    Serial.println(distance);
  }
  else // 거리 측정이 정상적으로 이뤄지지 않은 경우
  {
    Serial.println(" out of range ");
    distance = 9999;
  }

  
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    sprintf(apiURL,"http://15.165.204.254:8000/update?uuid=%s&distance=%d&mode=%s", uuid,distance,comm);
    //sprintf(apiURL,"http://192.168.35.204:8000/update?uuid=%s&distance=%d&mode=%s", uuid,distance,comm);
    Serial.println(apiURL);
    http.begin(apiURL);
    int httpCode = http.GET();
    Serial.println(httpCode);

    if (httpCode > 0) 
    {
      StaticJsonDocument<200> doc;
      
      String json = http.getString();
      DeserializationError error = deserializeJson(doc, json);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      String req_comm = doc["command"];
      Serial.println(req_comm);


      if (req_comm == "C000"){
        user_switch = 0;
      }
      if (req_comm == "C001"){
        user_switch = 1;
      }
      else if (req_comm == "C002"){
        user_switch = 2;
      }
      req_comm.toCharArray(comm, req_comm.length()+1);

      if (user_switch == 0){
        digitalWrite(r_pin, HIGH);
        digitalWrite(g_pin, HIGH);
        digitalWrite(b_pin, HIGH);
      }
      else if (user_switch == 1){
        if(distance <1000){
          digitalWrite(r_pin, HIGH);
          digitalWrite(g_pin, HIGH);
          digitalWrite(b_pin, LOW);
        }
        else{
          digitalWrite(r_pin, HIGH);
          digitalWrite(g_pin, HIGH);
          digitalWrite(b_pin, HIGH);
        }
      }
      
      
    }
    http.end(); //Close connection
  }
  delay(1000);
}
