#include "protocaldataitem.h"

ProtocalDataItem::ProtocalDataItem(VAR_TYPE type, QString name) :
    QStandardItem (name),
    type(type),
    name(name)
{
    this->setCheckable(true);
    //Implement editable later.
}

VAR_TYPE ProtocalDataItem::getType() const
{
    return type;
}

QString ProtocalDataItem::getName() const
{
    return name;
}

QLineSeries *ProtocalDataItem::getSeries() const
{
    return series;
}

void ProtocalDataItem::setSeries(QLineSeries *value)
{
    series = value;
}

void ProtocalDataItem::clearSeries()
{
    series->clear();
}

void ProtocalDataItem::setCurrentValue(double value)
{
    currentValue = value;
}

void ProtocalDataItem::onGraphUpdate(int currentX)
{
    if (checkState() == Qt::CheckState::Checked) {
        series->append(currentX, currentValue);
        if (series->count() > 100)
            series->remove(0);
    }
}
