// Test sketch — LCD 16x2 I2C only
// Default Wire pins on ESP32-S3, no remap needed

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_SDA   8
#define LCD_SCL   9
#define LCD_ADDR  0x27   // try 0x3F if blank
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

void setup() {
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("LCD Test OK");
  lcd.setCursor(0, 1); lcd.print("IO8 SDA/IO9 SCL");
}

void loop() {
  // static message, nothing to update
}
