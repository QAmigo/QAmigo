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
    Decoder(QObject *object, QList<Protocal *> &listProtocals);
    ~Decoder();

signals:
    void frameReady(int id, QList<double> listValues);		//Sends one ready frame out.
    void rawDataReady(const QByteArray &array);	//Sends what it recives.

public:
    void setConnection(QIODevice *connection);

//public slots:
//    void onDecodedParametersUpdated(const QList<Protocal *> &listProtocals);

    ENDIANESS getEndianess() const;
    void setEndianess(const ENDIANESS &value);

private slots:
    void dataReady();

private:
    void decode_buffer();
    void decodeFrame(int id, QByteArray frameRawData);

private:
    QByteArray buffer;
    QIODevice *connection;
    QList<Protocal *> &listProtocals;
    ENDIANESS endianess;
};

#endif // DECODERTHREAD_H
