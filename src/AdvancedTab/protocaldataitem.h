#ifndef PROTOCALDATAITEM_H
#define PROTOCALDATAITEM_H

#include <QStandardItem>

#include "vartype.h"

class ProtocalDataItem : public QStandardItem
{
public:
    ProtocalDataItem(VAR_TYPE type, QString name);

    VAR_TYPE getType() const;

    QString getName() const;

    QLineSeries *getSeries() const;
    void setSeries(QLineSeries *value);
    void clearSeries();

    void setCurrentValue(double value);

    void onGraphUpdate(int currentX);

private:
    VAR_TYPE type;
    QString name;

    QLineSeries *series;
    double currentValue;
};

#endif // PROTOCALDATAITEM_H
