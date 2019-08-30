#include "serialsendbox.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

SerialSendBox::SerialSendBox(QWidget *parent, QSerialPort *port) : QWidget(parent),
    buttonSend(new QPushButton("Send", this)),
    radioHex(new QRadioButton("Hex", this)),
    radioASC(new QRadioButton("ASC", this)),
    boxSend(new QPlainTextEdit(this)),
    port(port)
{
    radioASC->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(boxSend);

    QHBoxLayout *layoutControls = new QHBoxLayout(this);
    layoutControls->addWidget(radioASC);
    layoutControls->addWidget(radioHex);
    layoutControls->addWidget(buttonSend);
    layout->addLayout(layoutControls);
    setLayout(layout);

    connect(buttonSend, &QPushButton::clicked, this, &SerialSendBox::on_buttonSend_clicked);
    connect(radioASC, &QRadioButton::clicked, this, &SerialSendBox::onCheckBoxChanged);
    connect(radioHex, &QRadioButton::clicked, this, &SerialSendBox::onCheckBoxChanged);
}

int8_t SerialSendBox::toHex(char data)
{
    if (data <= '9' && data >= '0')
        return data - '0';
    if (data <= 'f' && data >= 'a')
        return data - 'a' + 10;

    return -1;
}

char SerialSendBox::parseHex(QByteArray *array)
{
    if (array->size() >= 2) {
        QByteArray hexArray = array->left(2);
        array->remove(0, 2);
        int8_t a, b;
        a = toHex(hexArray[0]);
        b = toHex(hexArray[1]);
        if (a >=0 && b >=0) {
            return a * 16 + b;
        }
    }

    return -1;
}

void SerialSendBox::on_buttonSend_clicked()
{
    if (port->isOpen()) {
        if (radioASC->isChecked()) {
            QString text = boxSend->toPlainText();
            port->write(text.toUtf8());
            emit addSendCount(text.count());
        } else {
            QByteArray bufferSend;
            QByteArray *bufferInput = new QByteArray(boxSend->toPlainText().remove(' ').toLower().toUtf8());
            while (bufferInput->size() != 0) {
                char hex = parseHex(bufferInput);
                if (hex >= 0) {
                    bufferSend.append(hex);
                } else {
                    emit errorSend("Inputs are not hex format numbers. Remember they have to presented in pairs.");
                    return;
                }
            }
            if (bufferSend.size() != 0) {
                port->write(bufferSend);
                emit addSendCount(bufferSend.count());
            }
        }
    }
}

void SerialSendBox::onCheckBoxChanged()
{
    boxSend->setPlainText("");
}
