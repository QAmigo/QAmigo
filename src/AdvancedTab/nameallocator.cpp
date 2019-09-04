#include "nameallocator.h"

const QString name_pool[COUNT_NAME] = {
    "alpha",
    "beta",
    "gamma",
    "delta",
    "epsilon",
    "zeta",
    "eta",
    "theta",
    "iota",
    "kappa",
    "lambda",
    "mu",
    "nu",
    "xi",
    "omicron",
    "pi",
    "rho",
    "sigma",
    "tau",
    "upsilon",
    "phi",
    "chi",
    "psi",
    "omega",
};

NameAllocator::NameAllocator()
{
    for (int i = 0; i < COUNT_NAME; i++)
        usageTable[i] = false;
}

QString NameAllocator::allocateName()
{
    int i = 0;
    while (i < COUNT_NAME) {
        if (usageTable[i] == false) {
            usageTable[i] = true;
            return name_pool[i];
        }
        i++;
    }

    return nullptr;
}

void NameAllocator::freeName(QString name)
{
    int id;
    if ((id = idFromName(name)) != -1)
        usageTable[id] = false;
}

void NameAllocator::setNameUsed(QString name)
{
    int id;
    if ((id = idFromName(name)) != -1)
        usageTable[id] = true;
}

int NameAllocator::idFromName(QString name)
{
    for (int i = 0; i < COUNT_NAME; i++) {
        if (name_pool[i].compare(name) == 0)
            return i;
    }

    return -1;
}
