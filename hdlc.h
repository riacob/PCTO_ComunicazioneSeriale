/*
 *
 * @authors Riccardo Iacob, Don Davide
 * @date 24/05/2022
 * @description HDLC interface
 *
 */

#ifndef HDLC_H
#define HDLC_H

#include <QObject>
#include <QByteRef>

typedef unsigned char Byte;

// HDLC with x16+x15+x2+1 poly
// CRC16-USB or CRC16-MODBUS

/*
HDLC pag 121
Byte stuffing pag 101

DATI = 8bFLG / 8bADD 8bCTR xbDAT 16bFCS / 8bFLG
PACCHETTO = 8bADD 8bCTR xbDAT 16bFCS

FLG = 0x7E
ESC = 0x7D

1. Inserire FLG
2. Preparare il pacchetto
    a. calcolare il CRC16 (FCS)
    b. inserire ADDR, ESC, CTR, DAT, FCS
4. Controllare l'eventuale presenza nel pacchetto di caratteri FLG, ESC
5. In caso affermativo
    a. Inserire il carattere di escape prima del carattere individuato
    b. Calcolare lo XOR tra il carattere individuato e 0x20 (complemento a 5)
6. Ritornare il pacchetto nel frame
*/

class HDLC
{
public:
    typedef struct decodedHDLC {
        Byte ADD;
        Byte CTR;
        QByteArray DAT;
        QByteArray FCS;
    } decodedHDLC;
    /*
     *
     * QByteArray encodeHDLC(Byte ADD, Byte CTR, QByteArray DAT)
     * @description encodes data trough HDLC
     * @params
     * - ADD the device address
     * - CTR the control command
     * - DAT the data to be transmitted
     * @returns nothing
     *
     */
    QByteArray encodeHDLC(Byte ADD, Byte CTR, QByteArray DAT);
    decodedHDLC decodeHDLC(QByteArray encodedHDLC);
};

#endif // HDLC_H
