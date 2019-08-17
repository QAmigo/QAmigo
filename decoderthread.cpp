#include "decoderthread.h"

DecoderThread::DecoderThread(Connection *connection,
                 const QByteArray *header,
                 const QList<VarType> *typeList):
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
    QByteArray array = connection->receive();

    buffer.append(array);
    emit rawDataReady(array);
    if (typeList != nullptr)
        decode_buffer();
}

void DecoderThread::decode_buffer()
{
    if (buffer.size() >= frameLength) {
        emit frameReady(buffer.left(frameLength));
        buffer = buffer.remove(0, frameLength);
    }
}
