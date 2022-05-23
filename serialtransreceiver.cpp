#include "serialtransreceiver.h"

SerialTransreceiver::SerialTransreceiver()
{
    serialPortSettings = new SerialPortSettings;
    serialPort = new QSerialPort();
}

SerialTransreceiver::~SerialTransreceiver()
{
    delete serialPortSettings;
    delete serialPort;
}

void SerialTransreceiver::setDefaultConfig()
{
    // Check if the serial port exists
    if (serialPort == NULL) {
        return;
    }

}

bool SerialTransreceiver::validateConfig()
{
    return true;
}

void SerialTransreceiver::openSerialPort()
{

}
