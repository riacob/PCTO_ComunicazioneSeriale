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


void MainWindow::on_pushButton_clicked()
{
    serial->writeTestChars();
}


void MainWindow::on_pushButton_2_clicked()
{
    serial->setPortName("COM5");
    serial->openSerialPort();
}


void MainWindow::on_pushButton_3_clicked()
{
    serial->readData();
}


void MainWindow::on_pushButton_4_clicked()
{
    serial->writeData(ui->plainTextEdit->toPlainText().toStdString().c_str());
}

