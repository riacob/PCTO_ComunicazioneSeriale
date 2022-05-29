#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define kBufferSize 100
#define ESC '}'
#define FLG '~'

struct HDLCData
{
  uint8_t ADD;
  uint8_t CTR;
  uint8_t recFCS[2];
  uint8_t calcFCS[2];
  uint8_t DAT[kBufferSize];
  int datLen;
  bool dataValid;
};

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint16_t crc16modbus(const char *dat, unsigned int len)
{
  uint16_t crc = 0xFFFF;
  unsigned int i = 0;
  char bit = 0;
  for (i = 0; i < len; i++)
  {
    // qDebug() << (char)dat[i];
    crc ^= dat[i];

    for (bit = 0; bit < 8; bit++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  // qDebug() << "CRC" << crc;
  String dt = dat;
  Serial.print(dt);
  Serial.print(" ");
  Serial.println(len);
  // sprintf("crc16modbus - DATA: %s - LEN: %d\n", dt.substring(0, len).c_str(), len);
  return crc;
}

void clearArray(char *arr, int len)
{
  for (int i = 0; i < len; i++)
  {
    arr[len] = 0;
  }
}

bool handleSerialEvent(char *dataBuffer)
{
  // Scan for incoming serial data, return true if it was found
  int idx = 0;
  bool serialAvailable = false;
  while (Serial.available() > 0)
  {
    dataBuffer[idx] = Serial.read();
    idx++;
    serialAvailable = true;
  }
  return serialAvailable;
}

void decodeHDLC(char *dataBuffer, HDLCData *dataStruct, bool dataReady = true)
{

  // Framed data length, including the flags
  int framedLen = 1;
  // Raw data length, without the flags
  int rawLen = 0;
  // Byte de-stuffing offset
  int stuffOfst = 0;
  // Received FCS bytes
  // uint8_t recFCS[2];
  // Destuffed data length
  int destuffedLen = 0;

  // If data is not ready we return
  if (!dataReady)
  {
    return;
  }

  // Find the length of the framed data, including the flags
  while (dataBuffer[framedLen] != FLG)
  {
    framedLen++;
  }
  // If it is not a single char, we need to account for the final flag
  if (framedLen > 1)
  {
    framedLen += 1;
  }

  // If it is a single char, ... and return; else we continue to decode the incoming data
  if (framedLen == 1)
  {
    return;
  }

  // Remove the frame and shift the array to the left
  rawLen = framedLen - 2;
  for (int i = 0; i < rawLen; i++)
  {
    dataBuffer[i] = dataBuffer[i + 1];
  }

  // De-stuff the array
  for (int j = 0; j < rawLen - stuffOfst; j++)
  {
    // Look for escape chars
    if (dataBuffer[j] == ESC)
    {
      // XOR the char after che excape char with 0x20 to obtain the original char
      dataBuffer[j + 1] ^= 0x20;
      // Shift the array to the left
      for (int k = j; k < rawLen - stuffOfst; k++)
      {
        dataBuffer[k] = dataBuffer[k + 1];
      }
      // Increase the stuff offset
      stuffOfst++;
    }
  }
  destuffedLen = rawLen - stuffOfst;

  // Save the 2 FCS bytes and remove them
  // Lenght == index + 1
  dataStruct->recFCS[0] = dataBuffer[destuffedLen - 2];
  dataStruct->recFCS[1] = dataBuffer[destuffedLen - 1];
  Serial.println(destuffedLen);

  // Calculate FCS (CRC16-MODBUS)
  String str = dataBuffer;
  String str1 = str.substring(0, destuffedLen);
  uint16_t calcFCS = crc16modbus(str1.c_str(), destuffedLen - 2);
  Serial.println(calcFCS, HEX);
  dataStruct->calcFCS[0] = calcFCS & 0xFF;
  dataStruct->calcFCS[1] = calcFCS >> 8;

  Serial.println("RFCS ");
  Serial.print((int)dataStruct->recFCS[0]);
  Serial.print(" ");
  Serial.println((int)dataStruct->recFCS[1]);
  Serial.println("CFCS ");
  Serial.println(str1);
  Serial.print((int)dataStruct->calcFCS[0]);
  Serial.print(" ");
  Serial.println((int)dataStruct->calcFCS[1]);

  // Compare FCSs
  int rF0 = (int)dataStruct->recFCS[0];
  int rF1 = (int)dataStruct->recFCS[1];
  int cF0 = (int)dataStruct->calcFCS[0];
  int cF1 = (int)dataStruct->calcFCS[1];
  if ((rF0 == cF0) && (rF1 == cF1))
  {
    dataStruct->dataValid = true;
  }
  else
  {
    dataStruct->dataValid = false;
  }

  // Save ADD and CTR
  dataStruct->ADD = dataBuffer[0];
  dataStruct->CTR = dataBuffer[2];

  // Save DAT
  dataStruct->datLen = destuffedLen - 5;
  for (int l = 3; l < dataStruct->datLen + 3; l++)
  {
    dataStruct->DAT[l - 3] = dataBuffer[l];
  }
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(50);
}

void loop()
{
  // Buffer containing serial data
  char serialBuffer[kBufferSize];
  clearArray(serialBuffer, kBufferSize);
  bool dataReady = false;
  HDLCData hdlcData;

  // When available, read all the incoming serial data
  dataReady = handleSerialEvent(serialBuffer);

  // Decode the serial data
  decodeHDLC(serialBuffer, &hdlcData, dataReady);

  // Print the decoded data
  if (dataReady)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    String hdlcstr = "";
    for (int i = 0; i < hdlcData.datLen; i++)
    {
      hdlcstr += (char)hdlcData.DAT[i];
    }
    lcd.print(hdlcstr);
    lcd.setCursor(0, 1);
    lcd.print("isValid:");
    lcd.setCursor(9, 1);
    lcd.print(hdlcData.dataValid);
  }

  // Wait a bit to avoid errors
  delay(50);
}

/*
  TODO:
  - account for the reception of single bytes
  - hdlc encoder
*/