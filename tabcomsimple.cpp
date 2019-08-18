#include "tabcomsimple.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <serialsendbox.h>

TabCOMSimple::TabCOMSimple(QWidget *parent,
                           QSerialPort *port) : QWidget(parent),
    boxReceive(new QPlainTextEdit()),
    radioReceiveHex(new QRadioButton("Hex")),
    radioReceiveASC(new QRadioButton("ASC")),
    buttonReceiveClear(new QPushButton("Clear Receive"))
{
    radioReceiveASC->setChecked(true);

    QHBoxLayout *layoutMain = new QHBoxLayout(this);
    QVBoxLayout *layoutReceive = new QVBoxLayout(this);
    QVBoxLayout *layoutSend = new QVBoxLayout(this);

    layoutMain->addLayout(layoutReceive);
    layoutMain->addLayout(layoutSend);

    layoutReceive->addWidget(boxReceive);
    QHBoxLayout *layoutReceiveControls = new QHBoxLayout(this);
    layoutReceive->addLayout(layoutReceiveControls);

    layoutReceiveControls->addWidget(radioReceiveASC);
    layoutReceiveControls->addWidget(radioReceiveHex);
    layoutReceiveControls->addWidget(buttonReceiveClear);

    for (int i = 0; i < 5; i++) {
        SerialSendBox *box = new SerialSendBox(this, port);
        layoutSend->addWidget(box);
        connect(box, &SerialSendBox::errorSend, this, &TabCOMSimple::sendError);
    }

    connect(buttonReceiveClear, &QPushButton::clicked, this, &TabCOMSimple::onButtonReceiveClearClicked);
}

void TabCOMSimple::rawDataReady(QByteArray array)
{
    boxReceive->moveCursor(QTextCursor::End);
    if (radioReceiveASC->isChecked())
        boxReceive->insertPlainText(array);
    else {
        uint16_t count = 0;
        for (char data : array.toHex()) {
            if (count % 2 == 1)
                boxReceive->insertPlainText(QString().sprintf("%c ", data));
            else
                boxReceive->insertPlainText(QString().sprintf("%c", data));
            count++;
        }
    }
}

void TabCOMSimple::onButtonReceiveClearClicked()
{
    boxReceive->setPlainText("");
}

void TabCOMSimple::sendError(QString string)
{
    emit errorMessage(string);
}
