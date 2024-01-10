#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QPushButton>
#include <QComboBox>
#include <QSerialPort>
#include "commdevice.h"

class SerialDevice : public CommDevice
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent,
                          QComboBox *comboPort,
                          QPushButton *buttonRefresh,
                          QComboBox *comboBaudrate,
                          QComboBox *comboDataBits,
                          QComboBox *comboParity,
                          QComboBox *comboStopBits,
                          QComboBox *comboFlowControl);
    ~SerialDevice() override;

private slots:
    void refreshPorts();
    void onErrorOccured();

private:
    QComboBox *comboPorts;
    QPushButton *buttonRefresh;
    QComboBox *comboBaudrate;
    QComboBox *comboDataBits;
    QComboBox *comboParity;
    QComboBox *comboStopBits;
    QComboBox *comboFlowControl;

    QSerialPort *port;

    // CommDevice interface
public:
    int open() override;
    void close() override;
};

#endif // SERIALDEVICE_H
