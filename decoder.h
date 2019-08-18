#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>
#include <QIODevice>

#include "vartype.h"

class Decoder : public QObject
{
    Q_OBJECT

public:
    Decoder(QObject *object, QIODevice *connection);
    Decoder(QObject *object, QIODevice *connection,
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

private slots:
    void dataReady();

private:
    void decode_buffer();

private:
    uint16_t frameLength;
};

#endif // DECODERTHREAD_H
