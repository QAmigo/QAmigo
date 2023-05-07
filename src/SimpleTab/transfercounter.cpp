#include "transfercounter.h"

TransferCounter::TransferCounter(QWidget *parent, QString name) :
    QWidget(parent),
    layout(new QHBoxLayout()),
    labelName(new QLabel(name)),
    countAll(0),
    labelCount(new QLabel("0")),
    timer(new QTimer(this))
{
    setLayout(layout);
    layout->addWidget(labelName);
    layout->addWidget(labelCount);

    connect(timer, &QTimer::timeout, this, &TransferCounter::onCalcSpeed);
    timer->start(1000);
}

QString readableFilesize(quint64 filesize)
{
    QStringList units;
    units << "B" << "KB" << "MB" << "GB" << "TB" << "PB";
    double mod  = 1024.0;
    double size = filesize;
    //qDebug() << size;
    int i = 0;
    long rest = 0;
    while (size >= mod && i < units.count()-1 )
    {
        rest= (long)size % (long)mod ;
        size /= mod;
        i++;
    }
    QString szResult = QString::number(floor(size));
    if( rest > 0)
    {
        szResult += QString(".") + QString::number(rest).left(2);
    }
    szResult += units[i];
    return  szResult;
}

void inline TransferCounter::updateLabel()
{
    labelCount->setText(QString().asprintf("%d ", countAll).append(readableFilesize(speed)).append("/s"));
}

void TransferCounter::add(int count)
{
    countAll += static_cast<uint32_t>(count);
    updateLabel();
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

void TransferCounter::onCalcSpeed()
{
    speed = countAll - lastCount;
    lastCount = countAll;
    if (speed != lastSpeed) {
        updateLabel();
    }
    lastSpeed = speed;
}

