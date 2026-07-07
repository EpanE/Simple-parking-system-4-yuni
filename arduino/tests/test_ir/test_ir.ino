// Test sketch — IR exit sensor only
// Prints raw state + interpreted obstacle/clear to Serial

#define IR_PIN 39
#define IR_OBSTACLE_STATE LOW  // most IR modules pull LOW when blocked — flip if inverted

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
}

void loop() {
  int state = digitalRead(IR_PIN);
  bool obstacle = (state == IR_OBSTACLE_STATE);
  Serial.printf("IR raw=%d -> %s\n", state, obstacle ? "OBJECT DETECTED" : "clear");
  delay(200);
}
