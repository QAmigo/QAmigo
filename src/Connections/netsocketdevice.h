#ifndef NETSOCKETDEVICE_H
#define NETSOCKETDEVICE_H

#include <QObject>
#include "commdevice.h"

#include <QLineEdit>
#include <QRadioButton>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>

class NetSocketDevice : public CommDevice
{
    Q_OBJECT
public:
    explicit NetSocketDevice(QObject *parent,
                             QLineEdit *inputIPAddr,
                             QLineEdit *inputPort,
                             QRadioButton *radioTcp,
                             QRadioButton *radioUdp,
                             QRadioButton *radioClient,
                             QRadioButton *radioServer);

    // CommDevice interface
public:
    int open() override;
    void close() override;

signals:
    void log(QString str);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError();

private:
    QLineEdit *inputIPAddr;
    QLineEdit *inputPort;
    QRadioButton *radioTcp;
    QRadioButton *radioUdp;
    QRadioButton *radioClient;
    QRadioButton *radioServer;

    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QAbstractSocket *socket;
};

#endif // NETSOCKETDEVICE_H
