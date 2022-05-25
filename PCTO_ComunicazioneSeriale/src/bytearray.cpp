#include "bytearray.h"
#include "Arduino.h"

void AByteArray::append(ByteArray destination, Byte b)
{
    int len = sizeof(destination) / sizeof(destination[0]);
    Serial.println(len);
    destination[len+1] = b;
}