#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>
#include <QIODevice>

#include "vartype.h"

class DecoderThread : public QThread
{
    Q_OBJECT

public:
    DecoderThread(QObject *object, QIODevice *connection);
    DecoderThread(QObject *object, QIODevice *connection,
            const QByteArray *header,
            const QList<VarType> *typeList);

private:
    QByteArray buffer;
    QByteArray frameHeader;
    const QList<VarType> *typeList;
    QIODevice *connection;

signals:
    void frameReady(QByteArray array);		//Sends one ready frame out.
    void rawDataReady(QByteArray array);	//Sends what it recives.

    // QThread interface
public:
    void run();

private:
    void decode_buffer();

private:
    uint16_t frameLength;
};

#endif // DECODERTHREAD_H
