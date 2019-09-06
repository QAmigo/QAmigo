#include "decoder.h"

Decoder::Decoder(QObject *object, QList<Protocal *> &listProtocals) :
    QObject (object),
    listProtocals(listProtocals)
{

}

Decoder::~Decoder()
{
}

void Decoder::setConnection(QIODevice *connection)
{
    connect(connection, &QIODevice::readyRead, this, &Decoder::dataReady);
    this->connection = connection;
}

void Decoder::dataReady()
{
    QByteArray array = connection->readAll();
    buffer.append(array);
    emit rawDataReady(array);
    if (listProtocals.count() != 0)
        decode_buffer();
}

void Decoder::decode_buffer()
{
    /*
     * This will search for frame headers and see if the length satisfys
     * if header exits.
     *
     * Exit condition:
     * 1. All data should be garbage if no frame header is found, remove it
     * then.
     * 2. If headers are found while no frame is retrived, indicates that
     * all frames are not complete.
     *
     * It would be another story if there are CRC bytes, will be added later.
     */
    bool found = false;
    bool retrived = false;
    do {
        found = false;
        retrived = false;
        for (int i = 0; i < listProtocals.count(); i++) {
            Protocal *protocal = listProtocals[i];
            int index;
            if ((index = buffer.indexOf(protocal->getHeader())) != -1) {
                if (buffer.length() - index >= protocal->getFrameLength()) {
                    decodeFrame(i, buffer.mid(index + protocal->getHeader().length(), protocal->getFrameLength()));
                    buffer.remove(index, protocal->getFrameLength());
                    retrived = true;
                }
                found = true;
            }
        }
        if (!found)
            buffer.clear();
    } while (found && retrived);
}

void Decoder::decodeFrame(int id, QByteArray frameRawData)
{
    Protocal *protocal = listProtocals.at(id);
    QList<VarType *> *listData = protocal->getListData();
    //Prepare data to send to plugins.
    QList<double> listValues;
    int count = 0;
    for (int i = 0; i < listData->count(); i++) {
        VarType *type = listData->at(i);
        type->setBufferValue(frameRawData.mid(count, type->getSize()));
        count += type->getSize();
        double value = type->getDouble(endianess);
        listValues.append(value);
    }
    emit frameReady(id, listValues);}

ENDIANESS Decoder::getEndianess() const
{
    return endianess;
}

void Decoder::setEndianess(const ENDIANESS &value)
{
    endianess = value;
}
