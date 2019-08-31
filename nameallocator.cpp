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
    int i = 0;
    while (i < COUNT_NAME) {
        if (usageTable[i] == true)
            if (name.compare(name_pool[i]) == 0) {
                usageTable[i] = false;
                return;
            }
        i++;
    }
}
