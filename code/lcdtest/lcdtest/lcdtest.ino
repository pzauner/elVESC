#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\nLCDisplay Test");
  lcd.init();
  lcd.backlight();
  delay(250);
  lcd.noBacklight();
  delay(1000); lcd.backlight(); delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  lcd.print("Sekunden seit Start:");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  delay(100);

}
