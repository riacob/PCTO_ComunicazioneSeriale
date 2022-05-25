#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define kBufferSize 100
#define FLG 0x7E
#define ESC 0x7D

struct HDLCData {
  uint8_t ADD;
  uint8_t CTR;
  uint8_t DAT[kBufferSize];
  uint8_t LEN;
  uint8_t DAT_LEN;
  bool dataValid;
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t serialBuffer[kBufferSize];
HDLCData hdlcData;

void serialBufferClear()
{
  for (int i= 0; i < kBufferSize; i++) {
    serialBuffer[i] = 0;
  }
}

void decodeHDLC(uint8_t* incomingData, HDLCData* dataStruct)
{

  if (incomingData[0] != '~') {
    return;
  }

  int len = 1;
  int lenData = 0;
  int removeOfst = 0;

  // Find the length (determined by the flags' position)
  while(incomingData[len] != '~') {
    len++;
  }
  len += 1;

  // Find the data lenght
  lenData = len - 8;

  Serial.print("Array size: ");
  Serial.println(len);
  Serial.print("Data length: ");
  Serial.println(lenData);

  // Byte de-stuffing
  for (int i = 1; i < len-removeOfst; i++) {
    if(incomingData[i] == ESC) {
      // XOR 0x20 the next char
      incomingData[i+1] ^= 0x20;
      // Remove the char and shift the remainder of the bytes to the left
      incomingData[i] = 0;
      for (int j = i; j < len-i-1; j++) {
        incomingData[j] = incomingData[j+1];
      }
      removeOfst++;
    }
  }

  // Print the de-stuffed data and the flags
  Serial.print("De-stuffed data: ");
  for (int k = 0; k < len; k++) {
    Serial.print(char(incomingData[k]));
    //Serial.print(" ");
  }
  Serial.println("");

  /*
  for (int i = 0; i < 20; i++) {
    Serial.print(incomingData[i], DEC);
    Serial.print(" ");
  }
  Serial.println("");
  */
  
  /*
  dataStruct->LEN = len;
  dataStruct->ADD = incomingData[1];
  dataStruct->CTR = incomingData[4];

  Serial.print("LEN ADD CTR ");
  Serial.print(len, DEC);
  Serial.print(" ");
  Serial.print(dataStruct->ADD, DEC);
  Serial.print(" ");
  Serial.print(dataStruct->CTR, DEC);
  Serial.print(" ");
  Serial.println("");

  // Data is from index 5, to data length + start index 5
  for (int j = 5; j < lenData+5; j++) {
    dataStruct->DAT[j-5] = incomingData[j];
    Serial.print(char(dataStruct->DAT[j-5]));
    Serial.print("");
  }
  Serial.println("");
  */
  // 

}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TXRX w/ HDLC");
  lcd.setCursor(0, 1);
  lcd.print("I.R., D.D., G.A.");
  delay(1000);
}

void loop()
{

  // While available, read the incoming serial data and put it into the serial data buffer
  int idx = 0;
  while (Serial.available())
  {
    serialBuffer[idx] = Serial.read();
    //Serial.print("LOOPIDX0 ");
    //Serial.println(serialBuffer[0], DEC);
    idx++;
  }

  // Decode the incoming data
  decodeHDLC(serialBuffer, &hdlcData);
  idx = 0;
  //serialBufferClear();
}