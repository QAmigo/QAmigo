#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>
#include <QIODevice>
#include <QList>
#include <QListWidget>

#include "vartypeitem.h"

class Decoder : public QObject
{
    Q_OBJECT

public:
    Decoder(QObject *object);
    ~Decoder();

signals:
    void frameReady(const QByteArray &array);		//Sends one ready frame out.
    void rawDataReady(const QByteArray &array);	//Sends what it recives.

public:
    void setConnection(QIODevice *connection);

public slots:
    void setDecodeParameters(const QByteArray header, const QListWidget &types);

private slots:
    void dataReady();

private:
    void decode_buffer();

private:
    QByteArray buffer;
    QByteArray frameHeader;
    QIODevice *connection;
    uint16_t frameLength;
    QList<VAR_TYPE> *listType;
};

#endif // DECODERTHREAD_H
