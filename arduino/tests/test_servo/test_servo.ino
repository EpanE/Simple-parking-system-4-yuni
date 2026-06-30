// Test sketch — SG90 servo only
// Requires "ESP32Servo" library (Arduino Library Manager)

#include <ESP32Servo.h>

#define SERVO_PIN 39
Servo gate;

void setup() {
  gate.attach(SERVO_PIN);
}

void loop() {
  gate.write(0);
  delay(1000);
  gate.write(90);
  delay(1000);
  gate.write(180);
  delay(1000);
}
