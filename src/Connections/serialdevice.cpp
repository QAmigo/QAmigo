#include "serialdevice.h"

#include <QtSerialPort/QSerialPortInfo>

#include <QMessageBox>

SerialDevice::SerialDevice(QObject *parent,
                           QComboBox *comboPorts,
                           QPushButton *buttonRefresh,
                           QComboBox *comboBaudrate,
                           QComboBox *comboDataBits,
                           QComboBox *comboParity,
                           QComboBox *comboStopBits,
                           QComboBox *comboFlowControl)
    : CommDevice(parent)
    , comboPorts(comboPorts)
    , buttonRefresh(buttonRefresh)
    , comboBaudrate(comboBaudrate)
    , comboDataBits(comboDataBits)
    , comboParity(comboParity)
    , comboStopBits(comboStopBits)
    , comboFlowControl(comboFlowControl)
    , port(new QSerialPort(this))
{
    refreshPorts();

    comboBaudrate->addItem("1200");
    comboBaudrate->addItem("2400");
    comboBaudrate->addItem("4800");
    comboBaudrate->addItem("9600");
    comboBaudrate->addItem("115200");
    comboBaudrate->setCurrentIndex(4);

    comboDataBits->addItem("5", 5);
    comboDataBits->addItem("6", 6);
    comboDataBits->addItem("7", 7);
    comboDataBits->addItem("8", 8);
    comboDataBits->setCurrentIndex(3);

    comboParity->addItem(tr("NoParity"),	0);
    comboParity->addItem(tr("EvenParity"),	2);
    comboParity->addItem(tr("OldParity"),	3);
    comboParity->addItem(tr("SpaceParity"),	4);
    comboParity->addItem(tr("MarkParity"),	5);
    comboParity->setCurrentIndex(0);

    comboStopBits->addItem("1",   1);
    comboStopBits->addItem("2",   2);
    comboStopBits->addItem("1.5", 3);
    comboStopBits->setCurrentIndex(0);

    comboFlowControl->addItem(tr("No"), 0);
    comboFlowControl->addItem(tr("Hard"), 1);
    comboFlowControl->addItem(tr("Soft"), 2);
    comboFlowControl->setCurrentIndex(0);

    setIODevice(port);

    connect(buttonRefresh, &QPushButton::clicked, this, &SerialDevice::refreshPorts);
    connect(port, &QSerialPort::errorOccurred, this, &SerialDevice::onErrorOccured);
}

SerialDevice::~SerialDevice()
{
    if (port->isOpen())
        port->close();
}

void SerialDevice::refreshPorts()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    comboPorts->clear();
    foreach (QSerialPortInfo info, infos) {
#ifdef Q_OS_LINUX
        comboPorts->addItem(QString("/dev/").append(info.portName()));
#else
        comboPorts->addItem(info.portName());
#endif
    }
}

int SerialDevice::open()
{
    if (comboPorts->currentText() == "")
        return -ENODEV;

    port->setPortName(comboPorts->currentText());
    bool ok;
    qint32 baudrate = comboBaudrate->currentText().toInt(&ok);
    if (ok)
        port->setBaudRate(baudrate);
    else {
        qDebug("baudrate parse failed");
        return -EINVAL;
    }

    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    switch(comboDataBits->currentData().toInt()) {
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
    switch (comboParity->currentData().toInt()) {
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
    switch (comboStopBits->currentData().toInt()) {
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
    switch (comboFlowControl->currentData().toInt()) {
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
        qDebug("port open failed");
        return -EIO;
    }
    emit connected();
    return 0;
}

void SerialDevice::close()
{
    if (port->isOpen()) {
        port->close();
    }
}

void SerialDevice::onErrorOccured()
{
    emit errorDisconnected();
}
