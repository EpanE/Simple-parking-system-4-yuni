// Test sketch — RC522 RFID only
// VSPI default pins on classic ESP32, no workaround needed

#include <SPI.h>
#include <MFRC522.h>

#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23
#define RC_SS     5
#define RC_RST    4
MFRC522 rfid(RC_SS, RC_RST);

void setup() {
  Serial.begin(115200);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, RC_SS);
  rfid.PCD_Init();
  Serial.println("RC522 ready — scan a card");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++)
      uid += String(rfid.uid.uidByte[i], HEX);
    uid.toUpperCase();
    Serial.println("Card UID: " + uid);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  delay(100);
}
