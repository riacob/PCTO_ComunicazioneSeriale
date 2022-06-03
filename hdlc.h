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

// Definition of "Byte", which is just an unsigned char
typedef unsigned char Byte;

class HDLC
{
public:

    // decodedHDLC struct, holds the data of a given decoded HDLC data packet
    typedef struct decodedHDLC {
        // Address
        Byte ADD;
        // Control
        Byte CTR;
        // Data
        QByteArray DAT;
        // CRC16
        QByteArray FCS;
        // Data validity flag, set if the calculated CRC16 matches the received one
        bool dataValid;
    } decodedHDLC;

    /*
     *
     * QByteArray encodeHDLC(Byte ADD, Byte CTR, QByteArray DAT)
     * @description encodes data trough HDLC
     * @params
     * - ADD the device address
     * - CTR the control command
     * - DAT the data to be transmitted
     * @returns the encoded data in a QByteArray
     *
     */
    static QByteArray encodeHDLC(Byte ADD, Byte CTR, QByteArray DAT);

    /*
     *
     * decodedHDLC decodeHDLC()
     * @description decodes data trough HDLC
     * @params none
     * @returns the decoded data in a decodedHDLC struct
     *
     */
    static decodedHDLC decodeHDLC(QByteArray encodedHDLC);

    /*
     * uint16_t crc16modbus(const char* dat, unsigned int len)
     * @description calculates the CRC16-MODBUS of the given dat array with the specified len
     * @params
     * - dat: the array of which to calculate the CRC16
     * - len: the length of the array
     *
     */
    static uint16_t crc16modbus(const char* dat, unsigned int len);

    /*
     *
     *  QByteArray cleanData(QByteArray data)
     *  @description removes everything from data except whatever is in between of two FLG chars
     *  @params
     *  - data: the data to clean
     *  @returns nothing
     *
     */
    static QByteArray cleanData(QByteArray data);

};

#endif // HDLC_H
