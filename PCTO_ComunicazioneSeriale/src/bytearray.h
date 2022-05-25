#ifndef BYTEARRAY_H
#define BYTEARRAY_H

typedef unsigned char Byte;

class ByteArray
{
    public:
    static void append(ByteArray destination, Byte b);
};

#endif