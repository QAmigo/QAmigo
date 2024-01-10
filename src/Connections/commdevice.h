#ifndef COMMDEVICE_H
#define COMMDEVICE_H

#include <QObject>
#include <QIODevice>

class CommDevice : public QObject
{
    Q_OBJECT
public:
    explicit CommDevice(QObject *parent);

    QIODevice *ioDevice;

    void setIODevice(QIODevice *ioDevice);
    virtual int open() = 0;
    virtual void close() = 0;
signals:

};

#endif // COMMDEVICE_H
