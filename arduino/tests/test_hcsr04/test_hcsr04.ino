// Test sketch — HC-SR04 x3 only
// Pins match main_system.ino — no other devices wired needed for this test

struct Sensor { uint8_t trig; uint8_t echo; const char* label; };
const Sensor sensors[] = {
  { 1, 2, "Front" },
  { 4, 5, "Left"  },
  { 6, 7, "Right" },
};
const int NUM_S = sizeof(sensors) / sizeof(sensors[0]);

float readDistance(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, LOW);  delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long d = pulseIn(echo, HIGH, 30000);
  return (d == 0) ? -1 : d * 0.034f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NUM_S; i++) {
    pinMode(sensors[i].trig, OUTPUT);
    pinMode(sensors[i].echo, INPUT);
    digitalWrite(sensors[i].trig, LOW);
  }
}

void loop() {
  for (int i = 0; i < NUM_S; i++) {
    float d = readDistance(sensors[i].trig, sensors[i].echo);
    Serial.printf("[%s] %s cm\n", sensors[i].label,
                  d < 0 ? "---" : String(d, 1).c_str());
    delay(30);
  }
  delay(300);
}
