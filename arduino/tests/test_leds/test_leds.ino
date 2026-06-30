// Test sketch — 3x Red + 3x Green LED only
// Verifies wiring/pin order by cycling each LED on then off in sequence

const uint8_t redPins[]   = { 15, 16, 17 }; // Slot Front, Left, Right
const uint8_t greenPins[] = { 18, 21, 38 }; // Slot Front, Left, Right
const int NUM_LEDS = 3;

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    digitalWrite(redPins[i], LOW);
    digitalWrite(greenPins[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(redPins[i], HIGH);
    delay(300);
    digitalWrite(redPins[i], LOW);
    digitalWrite(greenPins[i], HIGH);
    delay(300);
    digitalWrite(greenPins[i], LOW);
  }
}
