#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialtransreceiver.h"

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
}

