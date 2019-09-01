#ifndef DECODEDITEM_H
#define DECODEDITEM_H

#include <QCheckBox>
#include <QLabel>
#include <QLineSeries>
#include <QtCharts>
#include <QWidget>

class DecodedItem : public QWidget
{
    Q_OBJECT
public:
    explicit DecodedItem(QWidget *parent = nullptr, const QString &name = "");

    bool isCheckChanged();
    void clearState();
    QString getName();

signals:

public slots:
    void onCheckBoxClicked();

public:
    QCheckBox *checkShow;

    QLineSeries *getSeries() const;
    void setSeries(QLineSeries *value);

private:
    QLabel *labelName;	//Custom name?
    bool stateChanged;
    QLineSeries *series;
};

#endif // DECODEDITEM_H
