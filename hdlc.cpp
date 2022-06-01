/*
 *
 * @authors Riccardo Iacob, Don Davide
 * @date 24/05/2022
 * @description HDLC interface
 *
 */

#include "hdlc.h"

#include <QDebug>

#define FLG 0x7E // Flag char
#define ESC 0x7D // Esc char
#define FCS_LEN 2 // FCS length, 2 Bytes
#define BEG_LEN 3 // ADD + ESC + CTR length, 3 Bytes
#define FLG_LEN 1 // FLG length, 1 byte

/*

  Risorse utili:
    HDLC P.121
    Byte Stuffing P.111
    CRC P.28

 DATI = 8bFLG / 8bADD 8bESC 8bCTR xbDAT 16bFCS / 8bFLG
 ~ \x00}]\x00 HelloWorld \xEC \x00 ~
 PACCHETTO = 8bADD 8bESC 8bCTR xbDAT 16bFCS

 FLG = 0x7E
 ESC = 0x7D

 1. Inserire FLG all'inizio

 2. Preparare il pacchetto
    a. calcolare FCS, ottenuto da ADD + ESC + CTR + DAT processati dall'algoritmo del CRC16
    b. inserire nel frame ADDR + ESC + CTR + DAT + FCS

 3. Controllare l'eventuale presenza nel pacchetto di caratteri FLG, ESC

 4. In caso affermativo
    a. Inserire il carattere di escape prima del carattere individuato
    b. Calcolare lo XOR tra il carattere individuato e 0x20 (complemento a 5)

 5. Inserire FLG alla fine

 6. Consegnare il risultato

*/

QByteArray HDLC::encodeHDLC(Byte ADD, Byte CTR, QByteArray DAT)
{
    int DAT_LEN = DAT.length(); // Data length
    QByteArray out = 0; // Resulting framed packet
    QByteArray toChecksum = 0; // ADD + ESC + CTR + DAT

    // Start flag
    out[0] = FLG; // 1 Byte

    // Address
    out[1] = ADD; // 1 Byte
    // Is part of the to-checksum packet
    toChecksum.append(ADD);

    // Escape
    out[2] = ESC; // 1 Byte
    // Is part of the to-checksum packet
    toChecksum.append(ESC);

    // Control
    out[3] = CTR; // 1 Byte
    // Is part of the to-checksum packet
    toChecksum.append(CTR);

    // Data
    out.append(DAT); // DAT_LEN Byte(s)
    // Is part of the to-checksum packet
    toChecksum.append(DAT);

    // CRC16
    QString str = toChecksum.data();
    //qint16 crc = qChecksum(str.toStdString().c_str(), CRC_BITS);
    uint16_t crc = crc16modbus(str.toStdString().c_str(), str.length());
    // Split 16 bit number into 2 bytes
    Byte crc0 = crc & 0xFF;
    Byte crc1 = crc >> 8;
    //qDebug() << "CRCVAL " << crc;
    // Append the 2 bytes to the array
    out.append(crc0);
    out.append(crc1);

    // Byte stuffing
    // Loop trough the array excluding the start end end flags
    // Look for FLG and ESC chars
    // If found,
    // 1. Compute XOR between found char and 0x20
    // 2. Insert the ESC char before the found char
    // Continue
    // 8bADD 8bESC 8bCTR xbDAT 16bFCS
    for (int i = 1; i < BEG_LEN+DAT_LEN+FCS_LEN; i++) {
        if ((out[i].operator==(FLG)) || (out[i].operator==(ESC))) {
            out[i] = out[i] ^ 0x20;
            out.insert(i, ESC);
        }
    }

    // End flag
    out.append(FLG);

    return out;
}

HDLC::decodedHDLC HDLC::decodeHDLC(QByteArray encodedHDLC)
{
    HDLC::decodedHDLC decoded;
    // Index = DAT_LEN - FLG - BEG_LEN - FCS - FLG
    //int DAT_LEN = encodedHDLC.length() - 2 - 2 - 2 - 2;
    int escOfst = 0;;
    QByteArray toChecksum = 0;

    // Byte de-stuffing
    for (int i = 1; i < encodedHDLC.length()-escOfst-3; i++) {
        // If we find an escape char we xor the next char and remove the escape char
        // We must also offset the checksum position index
        if (encodedHDLC[i].operator==(ESC)) {
            encodedHDLC[i+1] = encodedHDLC[i+1] ^ 0x20;
            encodedHDLC.remove(i, 1);
            escOfst++;
        }
    }

    // Address
    decoded.ADD = encodedHDLC[1];

    // Control
    decoded.CTR = encodedHDLC[3];

    // Data
    for (int j = 4; j < encodedHDLC.length()-3; j++) {
        decoded.DAT.append(encodedHDLC[j]);
    }

    // Checksum
    decoded.FCS = 0;
    decoded.FCS[0] = encodedHDLC[encodedHDLC.length()-3];
    decoded.FCS[1] = encodedHDLC[encodedHDLC.length()-2];

    // Construct the array toChecksum
    toChecksum.append(decoded.ADD);
    toChecksum.append(ESC);
    toChecksum.append(decoded.CTR);
    toChecksum.append(decoded.DAT);
    toChecksum.append(decoded.FCS[0]);
    toChecksum.append(decoded.FCS[1]);
    // Verify checksum
    QString str = toChecksum.data();
    //qint16 crc = qChecksum(str.toStdString().c_str(), CRC_BITS);
    //qDebug() << str.toStdString().c_str();
    qint16 crc = crc16modbus(str.toStdString().c_str(), str.length()-2);
    // Split 16 bit number into 2 bytes
    Byte crc0 = crc & 0xFF;
    Byte crc1 = crc >> 8;
    //qDebug() << "CRCVAL " << crc;
    // Compare the received CRC16 to the calculated one, hence determine the data validity
    //qDebug() << (char)crc0 << (char)decoded.FCS[0];
    //qDebug() << (char)crc1 << (char)decoded.FCS[1];
    if (((char)crc0 == (char)decoded.FCS[0]) && ((char)crc1 == (char)decoded.FCS[1])) {
        decoded.dataValid = true;
    } else {
        decoded.dataValid = false;
    }

    return decoded;
}

uint16_t HDLC::crc16modbus(const char *dat, unsigned int len)
{

    uint16_t crc = 0xFFFF;
    unsigned int i = 0;
    char bit = 0;
    for( i = 0; i < len; i++ )
    {
        //qDebug() << (char)dat[i];
        crc ^= dat[i];

        for( bit = 0; bit < 8; bit++ )
        {
            if( crc & 0x0001 )
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
    //qDebug() << "crc16modbus - DATA:" << dat << "- LENGHT:" << len << "- CRC:" << crc << "- CRC[0]:" << (int)(crc & 0xFF) << "- CRC[1]:" << (int)(crc >> 8);
    return crc;
}

QByteArray HDLC::cleanData(QByteArray data)
{
    int packetLen = 0;
    int startIdx = 0;

    // If the data starts with FLG we already found the start
    // Which means we just need to find the second FLG
    // And it also means that we don't have to remove anything before it
    if (data.startsWith(FLG)) {
        packetLen = 1;
    }

    while (data[packetLen].operator!=(FLG)) {
        packetLen++;
        startIdx++;
    }
    packetLen++;

    // second parameter of mid function is length not index so we need to sum 1
    QByteArray cData = data.mid(startIdx, startIdx+packetLen+1);
    //cData = data.remove(packetLen+1, data.length()-packetLen);

    qDebug() << startIdx << cData;

    return cData;
}

