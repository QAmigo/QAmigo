#include "transfercounter.h"

TransferCounter::TransferCounter(QWidget *parent, QString name) :
    QWidget(parent),
    layout(new QHBoxLayout()),
    labelName(new QLabel(name)),
    countAll(0),
    labelCount(new QLabel("0"))
{
    setLayout(layout);
    layout->addWidget(labelName);
    layout->addWidget(labelCount);
}

void TransferCounter::add(int count)
{
    countAll += static_cast<uint32_t>(count);
    labelCount->setText(QString().sprintf("%d", countAll));
}

void TransferCounter::clear()
{
    countAll = 0;
    labelCount->setText("0");
}

void TransferCounter::setLabelText(QString text)
{
    labelName->setText(text);
}

