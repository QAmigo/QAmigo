#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>
#include <QIODevice>
#include <QList>
#include <QListWidget>

#include "protocal.h"
#include "vartype.h"

class Decoder : public QObject
{
    Q_OBJECT

public:
    Decoder(QObject *object, const QList<Protocal *> &listProtocals,
            const ENDIANESS &endianess);
    ~Decoder();

signals:
    void frameReady(int id, QList<double> listValues);		//Sends one ready frame out.
    void rawDataReady(const QByteArray &array);	//Sends what it recives.

public:
    void setConnection(QIODevice *connection);

private slots:
    void dataReady();

private:
    void decode_buffer();
    void decodeFrame(int id, QByteArray frameRawData);

private:
    QByteArray buffer;
    QIODevice *connection;
    const QList<Protocal *> &listProtocals;
    const ENDIANESS &endianess;
};

#endif // DECODERTHREAD_H
