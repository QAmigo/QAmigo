#include "decoder.h"

Decoder::Decoder(QObject *object, QIODevice *connection) :
    QObject(object),
    connection(connection),
    frameLength(0),
    listType(nullptr)
{
    connect(connection, &QIODevice::readyRead, this, &Decoder::dataReady);
}

Decoder::Decoder(QObject *object, QIODevice *connection,
                 const QByteArray *header,
                 const QListWidget *listType):
    QObject (object),
    connection(connection),
    frameLength(0),
    listType(listType)
{
    if (header != nullptr) {
        frameHeader = header->mid(0, 2);
        frameLength += 2;
    }
    frameHeader = nullptr;

//    for (VarType type : *typeList) {
//        frameLength += type.size;
//    }
}

void Decoder::dataReady()
{
    QByteArray array = connection->readAll();
    buffer.append(array);
    emit rawDataReady(array);
    if (listType != nullptr)
        decode_buffer();
}

void Decoder::decode_buffer()
{
    if (buffer.size() >= frameLength) {
        emit frameReady(buffer.left(frameLength));
        buffer = buffer.remove(0, frameLength);
    }
}
