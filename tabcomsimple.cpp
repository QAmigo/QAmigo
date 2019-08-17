#include "tabcomsimple.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <serialsendbox.h>

TabCOMSimple::TabCOMSimple(QWidget *parent, QSerialPort *port) : QWidget(parent),
    boxReceive(new QPlainTextEdit()),
    radioReceiveHex(new QRadioButton("Hex")),
    radioReceiveASC(new QRadioButton("ASC")),
    buttonReceiveClear(new QPushButton("Clear Receive"))
{
    QHBoxLayout *layoutMain = new QHBoxLayout(this);
    QVBoxLayout *layoutReceive = new QVBoxLayout(this);
    QVBoxLayout *layoutSend = new QVBoxLayout(this);

    layoutMain->addLayout(layoutReceive);
    layoutMain->addLayout(layoutSend);

    layoutReceive->addWidget(boxReceive);
    QHBoxLayout *layoutReceiveControls = new QHBoxLayout(this);
    layoutReceive->addLayout(layoutReceiveControls);

    layoutReceiveControls->addWidget(radioReceiveHex);
    layoutReceiveControls->addWidget(radioReceiveASC);
    layoutReceiveControls->addWidget(buttonReceiveClear);

    for (int i = 0; i < 5; i++)
        layoutSend->addWidget(new SerialSendBox(this, port));
}
