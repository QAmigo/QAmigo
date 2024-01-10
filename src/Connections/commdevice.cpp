#include "commdevice.h"

CommDevice::CommDevice(QObject *parent)
    : QObject(parent)
{}

void CommDevice::setIODevice(QIODevice *ioDevice)
{
    this->ioDevice = ioDevice;
}
