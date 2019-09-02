#include "protocal.h"

Protocal::Protocal(QByteArray header) :
    header(header),
    listData(new QList<VarType *>()),
    frameLength(header.length())
{
}

Protocal::~Protocal()
{
    delete listData;
}

void Protocal::append(VarType *type)
{
    listData->append(type);
    frameLength += type->getSize();
}

QList<VarType *> *Protocal::getListData() const
{
    return listData;
}

QByteArray Protocal::getHeader() const
{
    return header;
}

int Protocal::getFrameLength() const
{
    return frameLength;
}
