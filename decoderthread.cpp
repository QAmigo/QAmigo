#include "decoderthread.h"

DecoderThread::DecoderThread(QObject *object, QIODevice *connection) :
    QThread(object),
    typeList(nullptr),
    connection(connection),
    frameLength(0)
{

}

DecoderThread::DecoderThread(QObject *object, QIODevice *connection,
                 const QByteArray *header,
                 const QList<VarType> *typeList):
    QThread (object),
    typeList(typeList),
    connection(connection),
    frameLength(0)
{
    if (header != nullptr) {
        frameHeader = header->mid(0, 2);
        frameLength += 2;
    }
    frameHeader = nullptr;

    for (VarType type : *typeList) {
        frameLength += type.size;
    }
}

void DecoderThread::run()
{
    while (connection->isOpen()) {
        if (connection->waitForReadyRead(1000)) {
            QByteArray array = connection->readAll();

            buffer.append(array);
            emit rawDataReady(array);
            if (typeList != nullptr)
                decode_buffer();
        }
    }
}

void DecoderThread::decode_buffer()
{
    if (buffer.size() >= frameLength) {
        emit frameReady(buffer.left(frameLength));
        buffer = buffer.remove(0, frameLength);
    }
}
