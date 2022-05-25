#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int serialBuffer[100];
char serialBufferStr[100];

void serialBufferClear()
{
  for (int i= 0; i < 100; i++) {
    serialBuffer[i] = 0;
  }
}

void convertBuffer(int* serialBuffer, char* serialBufferStr, int cutoff)
{
  int i;
  for (i = 0; i < cutoff; i++) {
    serialBufferStr[i] = serialBuffer[i] - '0';
  }
  serialBufferStr[i+1] = '\0';
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
  String serialStr;
  while (Serial.available())
  {
    serialBuffer[idx] = Serial.read();
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print(serialBuffer);
    //Serial.print(serialBuffer[idx], DEC);
    //Serial.print(' ');
    serialStr += char(serialBuffer[idx]);
    Serial.println(serialStr);
    idx++;
  }
      //convertBuffer(serialBuffer, serialBufferStr, 5);
    //Serial.print(serialBufferStr);
  idx = 0;
  serialBufferClear();
}