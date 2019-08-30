#ifndef TRANSFERCOUNTER_H
#define TRANSFERCOUNTER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class TransferCounter : public QWidget
{
    Q_OBJECT
public:
    explicit TransferCounter(QWidget *parent = nullptr, QString *name = nullptr);

signals:

public slots:

public:
    void add(int count);
    void clear();

public:
    QHBoxLayout *layout;

private:
    QLabel *labelName;
    uint32_t countAll;
    QLabel *labelCount;
};

#endif // TRANSFERCOUNTER_H
