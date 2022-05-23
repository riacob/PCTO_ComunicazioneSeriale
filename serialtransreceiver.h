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
        // Open mode
        QSerialPort::OpenMode openMode = QSerialPort::ReadWrite;
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
    void setPortName(QString portName);

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
     * void setOpenMode(QSerialPort::OpenMode openMode)
     * @description sets the open mode of the serial port
     * @params
     * - openMode: set the serial port's open mode to this value
     * @returns nothing
     *
     */
    void setOpenMode(QSerialPort::OpenMode openMode);

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
     * - openMode: set the serial port's open mode to this value
     * @returns nothing
     *
     */
    void setSettings(QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::FlowControl flowControl, QSerialPort::Parity parity, QString portName, qint64 readBufferSize, QSerialPort::StopBits stopBits, QSerialPort::OpenMode openMode);

    /*
     *
     * void setDefaultConfig()
     * @description sets the serial port to the default configuration
     * @params none
     * @returns nothing
     *
     */
    void setDefaultSettings();

    /*
     *
     * void validateConfig()
     * @description validates the configuration
     * @params none
     * @returns boolean, if true configuration is valid, else it's to be considered invalid
     *
     */
    bool validateSettings();

    /*
     *
     * QSerialPort* getSerialPort()
     * @description returns the serial port object pointer, is very memory-unsafe
     * @params none
     * @returns the pointer to the QSerialPort object
     *
     */
    QSerialPort* getSerialPort();

    /*
     *
     * SerialPortSettings* getSettings()
     * @description returns the serial port settings struct pointer, is very memory-unsafe
     * @params none
     * @returns the pointer to the SerialPortSettings object
     *
     */
    SerialPortSettings* getSettings();

    /*
     *
     * QSerialPort* getSerialPort()
     * @description returns the serial port object, is very memory-unsafe
     * @params none
     * @returns the pointer to the QSerialPort object
     *
     */
    void openSerialPort();

    /*
     *
     * void closeSerialPort();
     * @description closes the serial port, if open
     * @params none
     * @returns nothing
     *
     */
    void closeSerialPort();

    /*
     *
     * void writeTestChars()
     * @description writes some characters to the serial port to verify that the communication between devices is wotking correctly
     * @params none
     * @returns nothing
     *
     */
    void writeTestChars();

    /*
     *
     * QByteArray readData()
     * @description reads the data coming from the serial port
     * @params none
     * @returns the QByteArray containing the received data
     *
     */
    QByteArray readData();

    /*
     *
     * void writeData(QByteArray data)
     * @description writes the given data to the serial port
     * @params none
     * @returns nothing
     *
     */
    void writeData(QByteArray data);
};

#endif // SERIALTRANSRECEIVER_H
