#include "serialconnection.h"
#include <QMessageBox>

SerialConnection::SerialConnection(QSerialPort *port) :
    port(port)
{
}

QByteArray SerialConnection::receive()
{
    port->waitForReadyRead(-1);
    return port->readAll();
}

void SerialConnection::send(QByteArray buffer)
{
    if (port->isOpen()) {
        port->write(buffer);
    } else {
        //errorString
    }
}
