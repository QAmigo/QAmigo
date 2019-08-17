#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QSerialPort>

#include "connection.h"

class SerialConnection : public Connection
{
public:
    SerialConnection(QSerialPort *port);

    // Connection interface
protected:
    QByteArray receive();
    void send(QByteArray buffer);

private:
    QSerialPort *port;
};

#endif // SERIALCONNECTION_H
