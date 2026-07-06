#define BLYNK_TEMPLATE_ID "TMPL3zv_6XG1W"
#define BLYNK_TEMPLATE_NAME "Kowsss Smart Home"
#define BLYNK_AUTH_TOKEN "Lr7oZnLbg2uN2K17wmj-9hOmZ3uELv3m"
#define BUZZER_PIN 19

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//WiFi Connection
char ssid[] = "kow";        // Wifi name
char pass[] = "kowkowkow";  // Wifi Password

// Pins
#define ONE_WIRE_BUS 4
#define LDR_PIN 34
#define TRIG 5
#define ECHO 18
#define LED_PIN 2      // Living room pin
#define LED2_PIN 15    // Bedroom pin
#define FAN_PIN 23

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//  Control variables
int mode = 1;        // 1 = AUTO,0 = MANUAL
int led1_state = 0;  // Living room
int led2_state = 0;  // Bedroom
int fan_state = 0;   // Fan

//  Blynk Controls
BLYNK_WRITE(V4) {   // Bedroom LED
  led2_state = param.asInt();
}

BLYNK_WRITE(V5) {   // Living room LED
  led1_state = param.asInt();
}

BLYNK_WRITE(V6) {   // Mode
  mode = param.asInt();
  Serial.println(mode);
}

BLYNK_WRITE(V7) {   // Fan
  fan_state = param.asInt();
}

void setup() {
  Serial.begin(115200);

  Serial.println("System Started...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  sensors.begin();

  lcd.init();
  lcd.backlight();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {

  // Serial.print("Mode: ");
  // Serial.println(mode);


  Blynk.run();

  //Temperature
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  //  Light
  int light = analogRead(LDR_PIN);
  String lightStatus;

  //  Distance
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  if(distance <= 0 || distance > 400)
    distance = 0;
   Serial.print("Light: ");
   Serial.println(light);
  

  // MAIN CONTROL LOGIC

  if(mode == 1) {   // AUTO MODE

    // Living Room (LDR)
    if(light > 2000) {
      digitalWrite(LED_PIN, HIGH);
      lightStatus = "DARK";
    } else {
      digitalWrite(LED_PIN, LOW);
      lightStatus = "BRIGHT";
    }

    // Bedroom (Ultrasonic)
    if(distance < 10 && distance > 0) {
      digitalWrite(LED2_PIN, HIGH);
    } else {
      digitalWrite(LED2_PIN, LOW);
    }
    Serial.println("Temp:");
    Serial.println(temp);

    if(temp > 33) {
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  }

  } else {   // MANUAL MODE

    digitalWrite(LED_PIN, led1_state);
    digitalWrite(LED2_PIN, led2_state);
    digitalWrite(FAN_PIN, fan_state);

  }

  //  Send to Blynk
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, distance);
  Blynk.virtualWrite(V2, lightStatus);


//  Scrolling top line
static int pos = 0;
String text = "   SMART HOME SYSTEM   ";

lcd.setCursor(0,0);
lcd.print(text.substring(pos, pos+16));

pos++;
if(pos > text.length() - 16) pos = 0;

//  Bottom line logic
lcd.setCursor(0,1);

//  1. Motion (highest priority)
if(distance < 10 && distance > 0) {
  lcd.print("MOTION DETECTED ");
}

// 2. Temperature display
else if(temp > 33) {
  lcd.print("TEMP:");
  lcd.print(temp);
  lcd.print("C   ");
}

//  3. Light status
else {
  if(light > 2000) {
    lcd.print("DARK | LIGHT ON ");
  } else {
    lcd.print("BRIGHT | OFF    ");
  }
}

Serial.println("------ SYSTEM STATUS ------");

Serial.print("Temperature: ");
Serial.println(temp);

Serial.print("Light Value: ");
Serial.println(light);

Serial.print("Light Status: ");
Serial.println(lightStatus);

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

Serial.print("Mode: ");
Serial.println(mode == 1 ? "AUTO" : "MANUAL");

Serial.print("Fan State: ");
Serial.println(digitalRead(FAN_PIN));

Serial.println("---------------------------");




  delay(1000);
}










