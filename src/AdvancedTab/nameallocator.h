#ifndef NAMEALLOCATOR_H
#define NAMEALLOCATOR_H

#include <QString>

class NameAllocator
{
#define COUNT_NAME 26
public:
    NameAllocator();

    QString allocateName();
    void freeName(QString name);
    void setNameUsed(QString name);

private:
    int idFromName(QString name);

private:
    bool usageTable[COUNT_NAME];
};

#endif // NAMEALLOCATOR_H
