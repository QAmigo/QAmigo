#ifndef CONNECTION_H
#define CONNECTION_H

#include <QByteArray>

class Connection
{
public:
    Connection();
    virtual ~Connection() {}

    virtual QByteArray receive() = 0;			//Receive as much as possible.
    virtual void send(QByteArray buffer) = 0;
};

#endif // CONNECTION_H
