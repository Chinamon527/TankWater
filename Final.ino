#include <Wire.h> //ไลบารี เชื่อมกับ I2C
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define ADS1115_ADDRESS 0x48  // Address A0
#define ADS1115_CONFIG_REG 0x01 // 
#define ADS1115_CONV_REG 0x00 //
#define BUZZER_PIN 5 //-
#define SENSOR_PIN 34

Servo myservo;
int buzzer = 5;
int sensor = 34;
int time4s=0;

const char* ssid = "OPPO";
const char* password = "22334455";
const char* serverUrl = "http://192.168.82.105:8000";

int val = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("เชื่อมต่อ WiFi...");
  }
  Serial.println("เชื่อมต่อ WiFi สำเร็จ!");

  pinMode(BUZZER_PIN, OUTPUT);
  delay(1000);
  writeRegister16(ADS1115_CONFIG_REG, 0x8483);
  myservo.attach(12);
}

void loop() {
  writeRegister16(ADS1115_CONFIG_REG, 0x8483);
  delay(100);
  int16_t adcValue = readRegister16(ADS1115_CONV_REG);
  float voltage_mV = (adcValue * 0.1875);
  int y = map(voltage_mV, -32, 74, 0, 100);
  String yString = String(y);
  getweb("/setto/" + yString);
  Serial.printf("\nVoltage: %.8f adcValue: %d\n", voltage_mV, adcValue);
  Serial.printf("percent: %d\n", y);

  val = analogRead(sensor);
  int x = map(val, 0, 2020, 0, 100);
  String xString = String(x);
  getweb("/setlevel/" + xString);
  Serial.printf("waterLv: %d\n", val);
  Serial.printf("percent: %d\n", x);

  String z = getweb("/servo");
  Serial.printf("z:=%d %s\n",z.toInt(),z);
  
  if(z.toInt()){
    servo();
    getweb("/do/0");
  }
  delay(1000);
}

void writeRegister16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

int16_t readRegister16(uint8_t reg) {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(ADS1115_ADDRESS, 2);

  if (Wire.available() >= 2) {
    int16_t value = Wire.read() << 8;
    value |= Wire.read();
    return value;
  }
  return 0;
}

void servo() {
  if(millis()-time4s >=1000){
    time4s = millis();
    myservo.write(0);
    delay(1000);
    myservo.write(30);
    delay(250);
  }
}

String getweb(String url) {
  HTTPClient http;
  http.begin(serverUrl + url);
  String response;
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    response = http.getString();
  } else {
    Serial.println("พบข้อผิดพลาดในการร้องขอ: " + http.errorToString(httpResponseCode));
  }
  http.end();
  return response;
}