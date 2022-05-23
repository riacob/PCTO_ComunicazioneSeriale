/*
 *
 * @authors Riccardo Iacob, Gorassini Alessandro
 * @date 23/05/2022
 * @description Serial trasmission and reception interface
 *
 */

#ifndef SERIALTRANSRECEIVER_H
#define SERIALTRANSRECEIVER_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialTransreceiver
{
private:
    typedef struct SerialPortSettings{
        // Baud rate
        QSerialPort::BaudRate baudRate = QSerialPort::Baud9600;
        // Data bits
        QSerialPort::DataBits dataBits = QSerialPort::Data8;
        // Flow control
        QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
        // Parity
        QSerialPort::Parity parity = QSerialPort::NoParity;
        // Parity
        QString portName;
        // Read buffer size
        qint64 readBufferSize = 0;
        // Stop bits
        QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    } SerialPortSettings;
    SerialPortSettings* serialPortSettings;
    QSerialPort* serialPort;
public:

    /*
     *
     * SerialTransreceiver()
     * @description initializes the class:
     * - Creates default serial settings object and assigns it to serialPortSettings pointer
     * @params none
     * @returns nothing
     *
     */
    SerialTransreceiver();

    /*
     *
     * SerialTransreceiver()
     * @description deallocates unused heap when the object is destroyed
     * - Deletes serial settings
     * @params none
     * @returns nothing
     *
     */
    ~SerialTransreceiver();

    /*
     *
     * void setBaudRate(QSerialPort::BaudRate baudRate)
     * @description sets the baudrate of the serial port
     * @params
     * - baudRate: set the serial port's baudrate to this value
     * @returns nothing
     *
     */
    void setBaudRate(QSerialPort::BaudRate baudRate);

    /*
     *
     * void setDataBits(QSerialPort::DataBits dataBits)
     * @description sets the data bits of the serial port
     * @params
     * - dataBits: set the serial port's data bits to this value
     * @returns nothing
     *
     */
    void setDataBits(QSerialPort::DataBits dataBits);

    /*
     *
     * void setFlowControl(QSerialPort::FlowControl flowControl)
     * @description sets the flow control of the serial port
     * @params
     * - flowControl: set the serial port's flow control to this value
     * @returns nothing
     *
     */
    void setFlowControl(QSerialPort::FlowControl flowControl);

    /*
     *
     * void setParity(QSerialPort::Parity parity)
     * @description sets the parity bits of the serial port
     * @params
     * - setParity: set the serial port's parity to this value
     * @returns nothing
     *
     */
    void setParity(QSerialPort::Parity parity);

    /*
     *
     * void setPortName(QString* portName)
     * @description sets the port name of the serial port
     * @params
     * - portName: set the serial port's name to this value
     * @returns nothing
     *
     */
    void setPortName(QString* portName);

    /*
     *
     * void setReadBufferSize(qint64 readBufferSize)
     * @description sets the port buffer size of the serial port
     * @params
     * - readBufferSize: set the serial port's buffer size to this value
     * @returns nothing
     *
     */
    void setReadBufferSize(qint64 readBufferSize);

    /*
     *
     * void setStopBits(QSerialPort::StopBits stopBits)
     * @description sets the stop bits of the serial port
     * @params
     * - stopBits: set the serial port's stop bits to this value
     * @returns nothing
     *
     */
    void setStopBits(QSerialPort::StopBits stopBits);

    /*
     *
     * setConfig(QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::FlowControl flowControl, QSerialPort::Parity parity, QString* portName, qint64 readBufferSize, QSerialPort::StopBits stopBits)
     * @description sets all the parameters of the serial port
     * @params
     * - baudRate: set the serial port's baudrate to this value
     * - dataBits: set the serial port's data bits to this value
     * - flowControl: set the serial port's flow control to this value
     * - parity: set the serial port's parity to this value
     * - portName: set the serial port's port name to this value
     * - readBufferSize: set the serial port's buffer size to this value
     * - setStopBits: set the serial port's stop bits to this value
     * @returns nothing
     *
     */
    void setConfig(QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::FlowControl flowControl, QSerialPort::Parity parity, QString* portName, qint64 readBufferSize, QSerialPort::StopBits stopBits);

    /*
     *
     * void setDefaultConfig()
     * @description sets the serial port to the default configuration
     * @params none
     * @returns nothing
     *
     */
    void setDefaultConfig();

    /*
     *
     * void validateConfig()
     * @description validates the configuration
     * @params none
     * @returns boolean, if true configuration is valid, else it's to be considered invalid
     *
     */
    bool validateConfig();

    QSerialPort* getSerialPort();
    SerialPortSettings* getSerialPortSettings();
    void openSerialPort();
    void closeSerialPort();
};

#endif // SERIALTRANSRECEIVER_H
