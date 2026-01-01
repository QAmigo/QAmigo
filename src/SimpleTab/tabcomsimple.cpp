#include "tabcomsimple.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>

#define CNT_SEND_BOX	5

TabCOMSimple::TabCOMSimple(QWidget *parent) : QWidget(parent),
    boxReceive(new QPlainTextEdit()),
    radioReceiveHex(new QRadioButton(tr("Hex"))),
    radioReceiveASC(new QRadioButton(tr("ASC"))),
    buttonReceiveClear(new QPushButton(tr("Clear All"))),
    counterRX(new TransferCounter(this, tr("RX"))),
    counterTX(new TransferCounter(this, tr("TX")))
{
    radioReceiveASC->setChecked(true);

    QHBoxLayout *layoutMain = new QHBoxLayout();
    setLayout(layoutMain);
    QVBoxLayout *layoutReceive = new QVBoxLayout();
    QVBoxLayout *layoutSend = new QVBoxLayout();

    layoutMain->addLayout(layoutReceive);
    layoutMain->addLayout(layoutSend);

    layoutReceive->addWidget(boxReceive);
    QHBoxLayout *layoutReceiveControls = new QHBoxLayout();
    layoutReceive->addLayout(layoutReceiveControls);

    layoutReceiveControls->addWidget(radioReceiveASC);
    layoutReceiveControls->addWidget(radioReceiveHex);

    layoutReceiveControls->addWidget(counterRX);
    layoutReceiveControls->addWidget(counterTX);

    layoutReceiveControls->addWidget(buttonReceiveClear);

    for (int i = 0; i < CNT_SEND_BOX; i++) {
        SendBox *box = new SendBox(this);
        layoutSend->addWidget(box);
        connect(box, &SendBox::errorSend, this, &TabCOMSimple::sendError);
        connect(box, &SendBox::addSendCount, this, &TabCOMSimple::addTXCount);
        sendBoxes.append(box);
    }

    connect(buttonReceiveClear, &QPushButton::clicked, this, &TabCOMSimple::onButtonReceiveClearClicked);
}

TabCOMSimple::~TabCOMSimple()
{
}

void TabCOMSimple::bindIODevice(QIODevice *ioDevice)
{
    foreach (SendBox *box, sendBoxes) {
        box->bindIODevice(ioDevice);
    }
}

void TabCOMSimple::rawDataReady(const QByteArray &array)
{
    /*
     * appendPlainText is much more faster than insert or any other methods. But in Qt,
     * QPlainTextEdit is designed as a log system, which means it will automatically
     * append new line when we append new text and this is inevitable, so better use it
     * as a log system until we can implement a better append method.
     */
    counterRX->add(array.size());
    boxReceive->appendPlainText(QTime().currentTime().toString("[hh:mm:ss.zzz]: "));
    if (radioReceiveASC->isChecked())
        boxReceive->appendPlainText(array);
    else {
        boxReceive->appendPlainText(array.toHex());
    }
}

void TabCOMSimple::addTXCount(int count)
{
    counterTX->add(count);
}

void TabCOMSimple::onButtonReceiveClearClicked()
{
    boxReceive->setPlainText("");
    counterRX->clear();
    counterTX->clear();
}

void TabCOMSimple::sendError(QString string)
{
    emit log(string);
}

void TabCOMSimple::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QWidget::changeEvent(event);
}

void TabCOMSimple::retranslateUi()
{
    radioReceiveHex->setText(QCoreApplication::translate("TabCOMSimple", "Hex"));
    radioReceiveASC->setText(QCoreApplication::translate("TabCOMSimple", "ASC"));
    buttonReceiveClear->setText(QCoreApplication::translate("TabCOMSimple", "Clear All"));
    counterRX->setLabelText(QCoreApplication::translate("TabCOMSimple", "RX"));
    counterTX->setLabelText(QCoreApplication::translate("TabCOMSimple", "TX"));
}
