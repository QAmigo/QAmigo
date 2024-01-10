#include "netsocketdevice.h"

#include <QRegularExpression>

NetSocketDevice::NetSocketDevice(QObject *parent, QLineEdit *inputIPAddr,
                                 QLineEdit *inputPort,
                                 QRadioButton *radioTcp, QRadioButton *radioUdp,
                                 QRadioButton *radioClient, QRadioButton *radioServer)
    : CommDevice(parent)
    , inputIPAddr(inputIPAddr)
    , inputPort(inputPort)
    , radioTcp(radioTcp)
    , radioUdp(radioUdp)
    , radioClient(radioClient)
    , radioServer(radioServer)
    , tcpSocket(new QTcpSocket(this))
    , udpSocket(new QUdpSocket(this))
    , socket(tcpSocket)
{
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &NetSocketDevice::onSocketError);
    connect(tcpSocket, &QAbstractSocket::connected, this, &NetSocketDevice::onSocketConnected);
    connect(udpSocket, &QAbstractSocket::errorOccurred, this, &NetSocketDevice::onSocketError);
    connect(udpSocket, &QAbstractSocket::connected, this, &NetSocketDevice::onSocketConnected);
}

int NetSocketDevice::open()
{
    // Check if input address and port is valid.
    static QRegularExpression ipRegex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");
    if (!ipRegex.match(inputIPAddr->text()).hasMatch()) {
        return -EINVAL;
    }
    bool ok;
    uint32_t port = inputPort->text().toUInt(&ok);
    if (!ok || port > 65535) {
        return -EINVAL;
    }

    if (radioTcp->isChecked()) {
        socket = tcpSocket;
    } else {
        socket = udpSocket;
    }
    if (radioClient->isChecked()) {
        socket->connectToHost(inputIPAddr->text(), static_cast<uint16_t>(port));
        emit log("connecting to " + inputIPAddr->text() + ":" + QString::asprintf("%d", port));
    }
    setIODevice(socket);

    return 0;
}

void NetSocketDevice::close()
{
    socket->close();
}

void NetSocketDevice::onSocketConnected()
{
    emit connected();
    emit log("connected.");
}

void NetSocketDevice::onSocketDisconnected()
{
    emit log("disconnected.");
}

void NetSocketDevice::onSocketError()
{
    emit errorDisconnected();
    emit log(QString("Error: ") + socket->errorString());
}
