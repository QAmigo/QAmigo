#ifndef PROTOCAL_H
#define PROTOCAL_H

#include "vartype.h"

class Protocal
{
public:
    Protocal(QByteArray header);
    ~Protocal();

    void append(VarType *type);

    QList<VarType *> *getListData() const;

    QByteArray getHeader() const;

    int getFrameLength() const;

private:
    QByteArray header;
    QList<VarType *> *listData;
    int frameLength;
};

#endif // PROTOCAL_H
