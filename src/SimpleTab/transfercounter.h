#ifndef TRANSFERCOUNTER_H
#define TRANSFERCOUNTER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>

class TransferCounter : public QWidget
{
    Q_OBJECT
public:
    explicit TransferCounter(QWidget *parent = nullptr, QString name = "");

signals:

public slots:

public:
    void add(int count);
    void clear();
    void setLabelText(QString text);

private slots:
    void onCalcSpeed();

public:
    QHBoxLayout *layout;

private:
    void inline updateLabel();

    QLabel *labelName;
    uint32_t countAll;
    QLabel *labelCount;

    QTimer *timer;
    uint32_t lastCount;
    uint32_t speed;
    uint32_t lastSpeed;
};

#endif // TRANSFERCOUNTER_H
