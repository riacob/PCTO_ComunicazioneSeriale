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
#define TST 0x99
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
    qint16 crc = qChecksum(str.toStdString().c_str(), 16);
    //qDebug() << "TOC1" << str.toStdString().c_str() << crc;
    // Split 16 bit number into 2 bytes
    Byte crc0 = crc & 0xFF;
    Byte crc1 = crc >> 8;
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
    //qDebug() << encodedHDLC.length();
    int DAT_LEN = encodedHDLC.length() - 2 - 2 - 2 - 2;
    int escOfst = 0;;
    QByteArray toChecksum = 0;

    // Address
    decoded.ADD = encodedHDLC[1];

    // Control
    decoded.CTR = encodedHDLC[4];

    // Data with reverse XOR 0x20 - result = charToXOR^0x20
    decoded.DAT = 0;
    for (int i = 5; i < DAT_LEN+5-escOfst; i++) {
        // If we find an escape char we xor the next char and remove the escape char
        // We must also offset the checksum position index
        if (encodedHDLC[i].operator==(ESC)) {
            encodedHDLC[i+1] = encodedHDLC[i+1] ^ 0x20;
            encodedHDLC.remove(i, 1);
            escOfst++;
        }
        decoded.DAT.append(encodedHDLC[i]);
    }

    // Checksum
    decoded.FCS = 0;
    decoded.FCS[0] = encodedHDLC[FCS_LEN+BEG_LEN+DAT_LEN-escOfst];
    decoded.FCS[1] = encodedHDLC[FCS_LEN+BEG_LEN+DAT_LEN+1-escOfst];

    // Verify checksum
    toChecksum.append(decoded.ADD);
    toChecksum.append(ESC);
    toChecksum.append(decoded.CTR);
    toChecksum.append(decoded.DAT);

    QString str = toChecksum.data();
    qint16 crc = qChecksum(str.toStdString().c_str(), 16);
    //qDebug() << "TOC2" << str.toStdString().c_str() << crc;
    // Split 16 bit number into 2 bytes
    Byte crc0 = crc & 0xFF;
    Byte crc1 = crc >> 8;
    //qDebug() << (char) decoded.FCS[0]  << (char) crc0 <<  (char) decoded.FCS[1]<<(char) crc1;
    if (((char)crc0 == (char)decoded.FCS[0]) && ((char)crc1 == (char)decoded.FCS[1])) {
        decoded.dataValid = true;
    } else {
        decoded.dataValid = false;
    }

    return decoded;
}

