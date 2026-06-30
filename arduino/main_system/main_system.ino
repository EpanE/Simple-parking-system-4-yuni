// ESP32-S3 DevKitC-1 — HC-SR04x3 + RC522 (whitelisted) + IR exit + LCD 16x2 I2C
//                       + 6x status LED + shared SG90 gate + optional Blynk IoT
// Cleaner than DEVKIT V1: no HSPI, no I2C remap, no GPIO12 concern

#include "blynk_config.h"   // must come before any Blynk library include
#if ENABLE_BLYNK
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#endif

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "rfid_whitelist.h"

// -- HC-SR04 + per-slot status LEDs --------------------
struct Sensor { uint8_t trig; uint8_t echo; uint8_t redPin; uint8_t greenPin; const char* label; };
const Sensor sensors[] = {
  { 1, 2, 15, 18, "Front" },
  { 4, 5, 16, 21, "Left"  },
  { 6, 7, 17, 38, "Right" },
};
const int NUM_S = sizeof(sensors) / sizeof(sensors[0]);
const float OCCUPIED_CM = 10.0f; // distance below this = slot occupied
bool slotOccupied[NUM_S] = { false, false, false };

// -- RC522 -- SPI2 default pins, no workaround needed --
#define SPI_SCK   12
#define SPI_MISO  13
#define SPI_MOSI  11
#define RC_SS     10
#define RC_RST    14
MFRC522 rfid(RC_SS, RC_RST);

// -- LCD I2C -- default Wire pins on S3 ----------------
#define LCD_SDA   8
#define LCD_SCL   9
#define LCD_ADDR  0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// -- IR sensor -- exit lane obstacle detect -------------
#define IR_PIN 40
#define IR_OBSTACLE_STATE LOW  // most IR modules pull LOW when blocked — flip if inverted

// -- Servo SG90 -- shared entry/exit gate ---------------
#define SERVO_PIN 39
Servo gate;
const int GATE_CLOSED = 0;
const int GATE_OPEN   = 90;
const unsigned long GATE_OPEN_MS = 3000;

#if ENABLE_BLYNK
unsigned long lastBlynkPush = 0;
#endif

float readDistance(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, LOW);  delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long d = pulseIn(echo, HIGH, 30000);
  return (d == 0) ? -1 : d * 0.034f / 2.0f;
}

void openGate() {
  gate.write(GATE_OPEN);
  delay(GATE_OPEN_MS);
  gate.write(GATE_CLOSED);
}

#if ENABLE_BLYNK
BLYNK_WRITE(V1) {
  if (param.asInt()) openGate(); // manual gate trigger from the app
}
#endif

void setup() {
  Serial.begin(115200);

  // SPI2 -- direct, no HSPI class needed on S3
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, RC_SS);
  rfid.PCD_Init();
  Serial.println("RC522 ready");

  // HC-SR04 + slot LEDs
  for (int i = 0; i < NUM_S; i++) {
    pinMode(sensors[i].trig, OUTPUT);
    pinMode(sensors[i].echo, INPUT);
    digitalWrite(sensors[i].trig, LOW);
    pinMode(sensors[i].redPin, OUTPUT);
    pinMode(sensors[i].greenPin, OUTPUT);
  }

  // IR exit sensor
  pinMode(IR_PIN, INPUT);

  // I2C -- no remap needed, IO8/IO9 are defaults on S3
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("System Ready    ");
  lcd.setCursor(0, 1); lcd.print("Scan RFID card  ");

  // Servo gate -- start closed
  gate.attach(SERVO_PIN);
  gate.write(GATE_CLOSED);

#if ENABLE_BLYNK
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
#endif
}

void loop() {
#if ENABLE_BLYNK
  Blynk.run();
#endif

  float dist[NUM_S];
  float minDist = 9999; int minIdx = 0;

  for (int i = 0; i < NUM_S; i++) {
    dist[i] = readDistance(sensors[i].trig, sensors[i].echo);
    slotOccupied[i] = (dist[i] > 0 && dist[i] < OCCUPIED_CM);
    digitalWrite(sensors[i].redPin, slotOccupied[i] ? HIGH : LOW);
    digitalWrite(sensors[i].greenPin, slotOccupied[i] ? LOW : HIGH);
    if (dist[i] > 0 && dist[i] < minDist) { minDist = dist[i]; minIdx = i; }
    Serial.printf("[%s] %s cm\n", sensors[i].label,
                  dist[i] < 0 ? "---" : String(dist[i], 1).c_str());
    delay(30);
  }

  lcd.setCursor(0, 1);
  if (minDist < 9999)
    lcd.printf("%-5s %5.1fcm    ", sensors[minIdx].label, minDist);
  else
    lcd.print("No object       ");

  // Entry — RFID, whitelist-gated
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++)
      uid += String(rfid.uid.uidByte[i], HEX);
    uid.toUpperCase();
    Serial.println("Card: " + uid);
    lcd.setCursor(0, 0);
    if (isWhitelisted(uid)) {
      lcd.print("Access OK       ");
      openGate();
    } else {
      lcd.print("Access Denied   ");
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // Exit — IR sensor, no auth needed, opens the same shared gate
  if (digitalRead(IR_PIN) == IR_OBSTACLE_STATE) {
    Serial.println("Exit IR triggered");
    openGate();
  }

#if ENABLE_BLYNK
  if (millis() - lastBlynkPush > 1000) {
    int freeSlots = 0;
    for (int i = 0; i < NUM_S; i++) if (!slotOccupied[i]) freeSlots++;
    Blynk.virtualWrite(V0, freeSlots);
    lastBlynkPush = millis();
  }
#endif

  delay(200);
}
