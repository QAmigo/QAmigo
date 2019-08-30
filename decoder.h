#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QThread>
#include <QIODevice>
#include <QListWidget>

#include "vartypeitem.h"

class Decoder : public QObject
{
    Q_OBJECT

public:
    Decoder(QObject *object, QIODevice *connection);
    Decoder(QObject *object, QIODevice *connection,
            const QByteArray *header,
            const QListWidget *listType);

private:
    QByteArray buffer;
    QByteArray frameHeader;
    QIODevice *connection;
    uint16_t frameLength;
    const QListWidget *listType;

signals:
    void frameReady(const QByteArray &array);		//Sends one ready frame out.
    void rawDataReady(const QByteArray &array);	//Sends what it recives.

private slots:
    void dataReady();

private:
    void decode_buffer();
};

#endif // DECODERTHREAD_H
