#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsendbox.h"
#include "tabcomsimple.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo info : infos)
        ui->comboPorts->addItem(info.portName());


    ui->comboBaudrate->addItem("1200", 1200);
    ui->comboBaudrate->addItem("2400", 2400);
    ui->comboBaudrate->addItem("4800", 4800);
    ui->comboBaudrate->addItem("9600", 9600);
    ui->comboBaudrate->addItem("115200", 115200);
    ui->comboBaudrate->setCurrentIndex(3);

    ui->comboDataBits->addItem("5", 5);
    ui->comboDataBits->addItem("6", 6);
    ui->comboDataBits->addItem("7", 7);
    ui->comboDataBits->addItem("8", 8);
    ui->comboDataBits->setCurrentIndex(3);

    ui->comboParity->addItem("NoParity",	0);
    ui->comboParity->addItem("EvenParity",	2);
    ui->comboParity->addItem("OldParity",	3);
    ui->comboParity->addItem("SpaceParity",	4);
    ui->comboParity->addItem("MarkParity",	5);
    ui->comboParity->setCurrentIndex(0);

    ui->comboStopBits->addItem("1",   1);
    ui->comboStopBits->addItem("2",   2);
    ui->comboStopBits->addItem("1.5", 3);
    ui->comboStopBits->setCurrentIndex(0);

    ui->comboFlowControl->addItem("No", 0);
    ui->comboFlowControl->addItem("Hard", 1);
    ui->comboFlowControl->addItem("Soft", 2);
    ui->comboFlowControl->setCurrentIndex(0);

    connect(ui->buttonOpen, &QPushButton::clicked, this, &MainWindow::openSerial);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::errorMessage(QString str)
{
    QMessageBox::warning(this, "Error", str);
}

void MainWindow::openSerial()
{
    if (ui->comboPorts->count() != 0) {
        if (port->isOpen()) {
            port->close();
            ui->buttonOpen->setText("Open");
        } else {
            port->setPortName(ui->comboPorts->currentText());
            port->setBaudRate(ui->comboBaudrate->itemData(ui->comboBaudrate->currentIndex()).Int);

            QSerialPort::DataBits dataBits = QSerialPort::Data8;
            switch(ui->comboDataBits->itemData(ui->comboDataBits->currentIndex()).Int) {
            case 5:
                dataBits = QSerialPort::Data5;
                break;
            case 6:
                dataBits = QSerialPort::Data6;
                break;
            case 7:
            dataBits = QSerialPort::Data7;
            break;
            case 8:
                dataBits = QSerialPort::Data8;
                break;
            default:
                qCritical("BUG: undefined serial databits.");
                break;
            }
            port->setDataBits(dataBits);

            QSerialPort::Parity parity = QSerialPort::NoParity;
            switch (ui->comboParity->itemData(ui->comboParity->currentIndex()).Int) {
            case 0:
                parity = QSerialPort::NoParity;
                break;
            case 2:
                parity = QSerialPort::EvenParity;
                break;
            case 3:
                parity = QSerialPort::OddParity;
                break;
            case 4:
                parity = QSerialPort::SpaceParity;
                break;
            case 5:
                parity = QSerialPort::MarkParity;
                break;
            default:
                qCritical("BUG: undefined serial parity.");
                break;
            }
            port->setParity(parity);

            QSerialPort::StopBits stopBits = QSerialPort::OneStop;
            switch (ui->comboStopBits->itemData(ui->comboStopBits->currentIndex()).Int) {
            case 1:
                stopBits = QSerialPort::OneStop;
                break;
            case 2:
                stopBits = QSerialPort::TwoStop;
                break;
            case 3:
                stopBits = QSerialPort::OneAndHalfStop;
                break;
            default:
                qCritical("BUG: undefined serial stopits.");
                break;
            }
            port->setStopBits(stopBits);

            QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
            switch (ui->comboFlowControl->itemData(ui->comboFlowControl->currentIndex()).Int) {
            case 0:
                flowControl = QSerialPort::NoFlowControl;
                break;
            case 1:
                flowControl = QSerialPort::HardwareControl;
                break;
            case 2:
                flowControl = QSerialPort::SoftwareControl;
                break;
            default:
                qCritical("BUG: undefined serial Flow Control Type.");
                break;
            }
            port->setFlowControl(flowControl);

            if (port->open(QIODevice::ReadWrite)) {
                QMessageBox::warning(this, "Serial open Failed.", "Port open Failed.");
            }
            ui->buttonOpen->setText("Close");

            ui->tabMain->addTab(new TabCOMSimple(this, port), "Simple");
        }
    }
}
