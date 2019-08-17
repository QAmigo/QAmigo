#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>

#include "connection.h"
#include "vartype.h"

class DecoderThread : QThread
{
    Q_OBJECT

public:
    DecoderThread(Connection *connection,
            const QByteArray *header,
            const QList<VarType> *typeList);

private:
    QByteArray buffer;
    QByteArray frameHeader;
    const QList<VarType> *typeList;
    Connection *connection;

signals:
    void frameReady(QByteArray array);		//Sends one ready frame out.
    void rawDataReady(QByteArray array);	//Sends what it recives.

    // QThread interface
protected:
    void run();

private:
    void decode_buffer();

private:
    uint16_t frameLength;
};

#endif // DECODERTHREAD_H
