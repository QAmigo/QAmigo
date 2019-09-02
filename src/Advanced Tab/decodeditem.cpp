#include "decodeditem.h"

#include <QGridLayout>

DecodedItem::DecodedItem(QWidget *parent, const QString &name) : QWidget(parent),
    checkShow(new QCheckBox()),
    labelName(new QLabel(name))
{
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->addWidget(labelName, 0, 0);
    layout->addWidget(checkShow, 0, 1);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    connect(checkShow, &QCheckBox::clicked, this, &DecodedItem::onCheckBoxClicked);
}

bool DecodedItem::isCheckChanged()
{
    return stateChanged;
}

void DecodedItem::clearState()
{
    stateChanged = false;
}

QString DecodedItem::getName()
{
    return labelName->text();
}

void DecodedItem::onCheckBoxClicked()
{
    stateChanged = true;
}

QLineSeries *DecodedItem::getSeries() const
{
    return series;
}

void DecodedItem::setSeries(QLineSeries *value)
{
    series = value;
}
