#ifndef HDLC_H
#define HDLC_H

#include <QObject>

// HDLC with x16+x15+x2+1 poly
// CRC16-USB or CRC16-MODBUS

class HDLC
{
public:
    static QByteArray frame(QByteArray data);
    static int crc16(char *frame, char framelgt);
    static QByteArray stuff(QByteArray data, int len);
public:
    HDLC();
    static QByteArray doHDLC(QByteArray data);
};

#endif // HDLC_H
