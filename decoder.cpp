#include "decoder.h"

Decoder::Decoder(QObject *object) :
    QObject(object),
    frameLength(0),
    listType(new QList<VAR_TYPE>())
{
}

void Decoder::setConnection(QIODevice *connection)
{
    connect(connection, &QIODevice::readyRead, this, &Decoder::dataReady);
    this->connection = connection;
}

void Decoder::setDecodeParameters(const QByteArray header, const QListWidget &types)
{
    frameLength = 0;
    if (header.size() == 1 || header.size() == 2) {
        frameHeader = header;
        frameLength += header.size();
    }
    //Raise error.

    listType->clear();
    for (int i = 0; i < types.count(); i++) {
        QListWidgetItem *item = types.item(i);
        VarTypeItem *varType = static_cast<VarTypeItem *>(item);

        listType->append(varType->type);
        frameLength += varType->getSize();
    }
}

void Decoder::dataReady()
{
    QByteArray array = connection->readAll();
    buffer.append(array);
    emit rawDataReady(array);
    if (listType->count() != 0)
        decode_buffer();
}

void Decoder::decode_buffer()
{
    while (buffer.size() >= frameLength) {
        int index = buffer.indexOf(frameHeader);
        if (index != -1) {
            if (index != 0)
                buffer.remove(0, index);
            if (buffer.size() >= frameLength) {
                emit frameReady(buffer.mid(frameHeader.size(), frameLength - frameHeader.size()));
                buffer.remove(0, frameLength);
            } else
                break;
        } else
            break;
    }
}
