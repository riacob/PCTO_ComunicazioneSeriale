#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
char serialBuffer[50];

void serialBufferClear()
{
  for (int i= 0; i < 50; i++) {
    serialBuffer[i] = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Transreceiver");
  lcd.setCursor(0, 1);
  lcd.print("Iacob, Gorassini");
  delay(1000);
}

void loop()
{
  int idx = 0;
  while (Serial.available())
  {
    serialBuffer[idx] = Serial.read();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(serialBuffer);
    idx++;
  }
  idx = 0;
  serialBufferClear();
}
