#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "serialtransreceiver.h"
#include "hdlc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new SerialTransreceiver();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Write test chars
void MainWindow::on_pushButton_clicked()
{
    serial->writeTestChars();
}


// Connect
void MainWindow::on_pushButton_2_clicked()
{
    serial->setPortName(ui->plainTextEdit_2->toPlainText());
    serial->openSerialPort();
}

// Read
void MainWindow::on_pushButton_3_clicked()
{
    QByteArray data = serial->readData();
    ui->plainTextEdit_3->clear();
    ui->plainTextEdit_3->appendPlainText(tr(data));
}

// Write
void MainWindow::on_pushButton_4_clicked()
{
    serial->writeData(ui->plainTextEdit->toPlainText().toStdString().c_str());
}

// Disconnect
void MainWindow::on_pushButton_5_clicked()
{
    serial->closeSerialPort();

    QString input = "hello";
    // char (int 135) = '?'
    int add = 135;
    int ctr = 76;
    QByteArray eHDLC = HDLC().encodeHDLC(add, ctr, input.toStdString().c_str());
    HDLC::decodedHDLC dHDLC = HDLC().decodeHDLC(eHDLC);
    qDebug() << HDLC().crc16modbus("a}ahelo", 7);
    qDebug() << HDLC().crc16modbus("a}ahelo�g", 7);

    /*
    qDebug() << "INPUT TO HDLC (ADD, CTR, DAT)" << (char)add << (char)ctr << input;
    qDebug()  << "ENCODED HDLC" << eHDLC.toStdString().c_str();
    qDebug() << "DECODED HDLC (ADD, CTR, DAT, VALID?)" << (char)dHDLC.ADD << (char)dHDLC.CTR << dHDLC.DAT << dHDLC.dataValid;
    qDebug() << "---------------------------------";
    qDebug() << "hello1" << HDLC().crc16modbus("?}]Lhello", 9);
    qDebug() << "hello2" << HDLC().crc16modbus("?}]Lhello(,", 9);
    //qDebug() << "hello3" << qChecksum("HELLO THIS IS JUST A TEST", 16);
    qDebug() << "---------------------------------";
    */
}

// Write w/ HDLC
void MainWindow::on_pushButton_6_clicked()
{
    serial->writeDataEncodeHDLC('a', 'a', ui->plainTextEdit->toPlainText().toStdString().c_str());
}

