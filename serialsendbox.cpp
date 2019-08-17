#include "serialsendbox.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

SerialSendBox::SerialSendBox(QWidget *parent, QSerialPort *port) : QWidget(parent),
    buttonSend(new QPushButton("Send", this)),
    radioHex(new QRadioButton("Hex", this)),
    radioASC(new QRadioButton("ASC", this)),
    boxSend(new QPlainTextEdit(this)),
    port(port)
{
    radioHex->setEnabled(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(boxSend);

    QHBoxLayout *layoutControls = new QHBoxLayout(this);
    layoutControls->addWidget(radioASC);
    layoutControls->addWidget(radioHex);
    layoutControls->addWidget(buttonSend);
    layout->addLayout(layoutControls);
    setLayout(layout);

    connect(buttonSend, &QPushButton::clicked, this, &SerialSendBox::on_buttonSend_clicked);
}

void SerialSendBox::on_buttonSend_clicked()
{
    if (port->isOpen()) {
        if (radioASC->isEnabled()) {
            port->write(boxSend->toPlainText().toUtf8());
        } else {
            //Write them as hex.
        }
    }
}
