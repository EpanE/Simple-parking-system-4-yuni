// Test sketch — LCD 16x2 I2C only
// Default Wire pins on classic ESP32, no remap needed

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_SDA   21
#define LCD_SCL   22
#define LCD_ADDR  0x27   // try 0x3F if blank
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

void setup() {
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("LCD Test OK");
  lcd.setCursor(0, 1); lcd.print("IO21 SDA/IO22 SCL");
}

void loop() {
  // static message, nothing to update
}
