/*
 *
 * @authors Riccardo Iacob, Gorassini Alessandro
 * @date 23/05/2022
 * @description Serial trasmission and reception interface
 *
 */

#include "serialtransreceiver.h"
#include "QtSerialPort/QSerialPort"
#include <QDebug>

SerialTransreceiver::SerialTransreceiver()
{
    serialPortSettings = new SerialPortSettings;
    serialPort = new QSerialPort();
    setDefaultSettings();
}

SerialTransreceiver::~SerialTransreceiver()
{
    delete serialPortSettings;
    delete serialPort;
}

void SerialTransreceiver::setDefaultSettings()
{
    // Check if the serial port exists
    if (serialPort == NULL) {
        return;
    }

    // Assign the serial port settings
    serialPort->setBaudRate(serialPortSettings->baudRate);
    serialPort->setDataBits(serialPortSettings->dataBits);
    serialPort->setFlowControl(serialPortSettings->flowControl);
    serialPort->setParity(serialPortSettings->parity);
    serialPort->setPortName(serialPortSettings->portName);
    serialPort->setReadBufferSize(serialPortSettings->readBufferSize);
    serialPort->setStopBits(serialPortSettings->stopBits);

}

bool SerialTransreceiver::validateSettings()
{
    //serialPort->setPortName("COM5");
    return true;
}

void SerialTransreceiver::openSerialPort()
{
    // Check if the config is valid
    if (!validateSettings()) {
        return;
    }
    serialPort->open(serialPortSettings->openMode);
    qDebug() << serialPort->portName() << serialPort->baudRate();
}

void SerialTransreceiver::closeSerialPort()
{
    // Check if serial port is open
    if (!serialPort->isOpen()) {
        return;
    }
    serialPort->close();
}

void SerialTransreceiver::writeTestChars()
{
    // Check if the serial port is open
    if (!serialPort->isOpen()) {
        return;
    }

    // Write a known char to ensure communication is ok
    serialPort->write("7");
}

QSerialPort* SerialTransreceiver::getSerialPort()
{
    return serialPort;
}

SerialTransreceiver::SerialPortSettings* SerialTransreceiver::getSettings()
{
    return serialPortSettings;
}

QByteArray SerialTransreceiver::readData()
{
    // Check if serial port is open
    if (!serialPort->isOpen()) {
        return 0;
    }

    // Read the incoming data from the serial port and return
    QByteArray data;
    data = serialPort->readAll();
    qDebug() << data;
    return data;
}

void SerialTransreceiver::writeData(QByteArray data)
{
    // Check if serial port is open
    if (!serialPort->isOpen()) {
        return;
    }

    // Write the data to the serial port
    serialPort->write(data);
}

void SerialTransreceiver::setBaudRate(QSerialPort::BaudRate baudRate)
{
    serialPortSettings->baudRate = baudRate;
    serialPort->setBaudRate(serialPortSettings->baudRate);
}

void SerialTransreceiver::setDataBits(QSerialPort::DataBits dataBits)
{
    serialPortSettings->dataBits = dataBits;
    serialPort->setDataBits(serialPortSettings->dataBits);
}

void SerialTransreceiver::setFlowControl(QSerialPort::FlowControl flowControl)
{
    serialPortSettings->flowControl = flowControl;
    serialPort->setFlowControl(flowControl);
}

void SerialTransreceiver::setParity(QSerialPort::Parity parity)
{
    serialPortSettings->parity = parity;
    serialPort->setParity(parity);
}

void SerialTransreceiver::setPortName(QString portName)
{
    serialPortSettings->portName = portName;
    serialPort->setPortName(serialPortSettings->portName);
}

void SerialTransreceiver::setReadBufferSize(qint64 readBufferSize)
{
    serialPortSettings->readBufferSize = readBufferSize;
    serialPort->setReadBufferSize(serialPortSettings->readBufferSize);
}

void SerialTransreceiver::setStopBits(QSerialPort::StopBits stopBits)
{
    serialPortSettings->stopBits = stopBits;
    serialPort->setStopBits(serialPortSettings->stopBits);
}

void SerialTransreceiver::setOpenMode(QSerialPort::OpenMode openMode)
{
    serialPortSettings->openMode = openMode;
}

void SerialTransreceiver::setSettings(QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::FlowControl flowControl, QSerialPort::Parity parity, QString portName, qint64 readBufferSize, QSerialPort::StopBits stopBits, QSerialPort::OpenMode openMode)
{
    // Save the settings
    serialPortSettings->baudRate = baudRate;
    serialPortSettings->dataBits = dataBits;
    serialPortSettings->flowControl = flowControl;
    serialPortSettings->parity = parity;
    serialPortSettings->portName = portName;
    serialPortSettings->readBufferSize = readBufferSize;
    serialPortSettings->stopBits = stopBits;
    serialPortSettings->openMode = openMode;

    // Apply the settings
    serialPort->setBaudRate(serialPortSettings->baudRate);
    serialPort->setDataBits(serialPortSettings->dataBits);
    serialPort->setFlowControl(serialPortSettings->flowControl);
    serialPort->setParity(serialPortSettings->parity);
    serialPort->setPortName(serialPortSettings->portName);
    serialPort->setReadBufferSize(serialPortSettings->readBufferSize);
    serialPort->setStopBits(serialPortSettings->stopBits);
}

void SerialTransreceiver::writeDataEncodeHDLC(Byte ADD, Byte CTR, QByteArray data)
{
    // Encode and write the data to the serial port
    QByteArray finalData = HDLC::encodeHDLC(ADD, CTR, data);
    qDebug() << "writeDataEncodeHDLC -" << finalData;
    writeData(finalData);
}

HDLC::decodedHDLC SerialTransreceiver::readDataDecodeHDLC()
{
    // Read and decode the data from the serial port
    HDLC::decodedHDLC decodedHDLC;
    QByteArray rData = readData();
    decodedHDLC = HDLC::decodeHDLC(rData);
    return decodedHDLC;
}
