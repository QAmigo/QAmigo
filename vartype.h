#ifndef VARTYPE_H
#define VARTYPE_H

#include <QString>

class VarType
{
public:
    VarType(QString name, uint8_t size) :
        name(name),
        size(size)
    {
    }

public:
    QString name;
    uint8_t size;
};

#endif // VARTYPE_H
