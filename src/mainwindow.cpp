#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    port(new QSerialPort()),
    decoder(new Decoder(this))
{
    ui->setupUi(this);

    refreshPorts();

    ui->comboBaudrate->addItem("1200");
    ui->comboBaudrate->addItem("2400");
    ui->comboBaudrate->addItem("4800");
    ui->comboBaudrate->addItem("9600");
    ui->comboBaudrate->addItem("115200");
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

    tabCOMSimple = new TabCOMSimple(this, port);
    ui->tabMain->addTab(tabCOMSimple, "Simple");

    tabAdvanced = new TabAdvanced(this);
    ui->tabMain->addTab(tabAdvanced, "Advanced");

    connect(ui->buttonOpen, &QPushButton::clicked, this, &MainWindow::openSerial);

    connect(tabCOMSimple, &TabCOMSimple::errorMessage, this, &MainWindow::errorMessage);
    connect(decoder, &Decoder::rawDataReady, tabCOMSimple, &TabCOMSimple::rawDataReady);
    connect(decoder, &Decoder::frameReady, tabAdvanced, &TabAdvanced::frameDataReady);
    connect(tabAdvanced, &TabAdvanced::sendDecodeParameters, decoder, &Decoder::setDecodeParameters);
}

MainWindow::~MainWindow()
{
    if (port->isOpen())
        port->close();
    delete port;
    delete ui;
}

void MainWindow::refreshPorts()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo info : infos)
        ui->comboPorts->addItem(info.portName());

}

void MainWindow::errorMessage(QString str)
{
    QMessageBox::warning(this, "Error", str);
}

void MainWindow::openSerial()
{
    if (ui->comboPorts->currentText() != "") {
        if (port->isOpen()) {
            port->close();
            ui->buttonOpen->setText("Open");
        } else {
            port->setPortName(ui->comboPorts->currentText());
            bool ok;
            qint32 baudrate = ui->comboBaudrate->currentText().toInt(&ok);
            if (ok)
                port->setBaudRate(baudrate);
            else {
                QMessageBox::warning(this, "Error", "Baudrate parse failed.");
                return;
            }

            QSerialPort::DataBits dataBits = QSerialPort::Data8;
            switch(ui->comboDataBits->currentData().toInt()) {
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
            switch (ui->comboParity->currentData().toInt()) {
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
            switch (ui->comboStopBits->currentData().toInt()) {
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
            switch (ui->comboFlowControl->currentData().toInt()) {
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

            if (!port->open(QIODevice::ReadWrite)) {
                QMessageBox::warning(this, "Serial open Failed.", "Port open Failed.");
                return;
            }
            ui->buttonOpen->setText("Close");

            decoder->setConnection(port);
        }
    }
}

void MainWindow::onButtonRefreshClicked()
{
    refreshPorts();
}
