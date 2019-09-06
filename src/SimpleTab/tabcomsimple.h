#ifndef TABCOMSIMPLE_H
#define TABCOMSIMPLE_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QPlainTextEdit>
#include <QSerialPort>
#include <QLabel>

#include "transfercounter.h"

class TabCOMSimple : public QWidget
{
    Q_OBJECT
public:
    explicit TabCOMSimple(QWidget *parent = nullptr,
                          QSerialPort *port = nullptr);
    ~TabCOMSimple();

private:
    void retranslateUi();

signals:
    void errorMessage(QString string);

public slots:
    void rawDataReady(const QByteArray &array);
    void addTXCount(int count);

private slots:
    void onButtonReceiveClearClicked();
    void sendError(QString string);

private:
    QPlainTextEdit *boxReceive;
    QRadioButton *radioReceiveHex;
    QRadioButton *radioReceiveASC;
    QPushButton *buttonReceiveClear;
    TransferCounter *counterRX;
    TransferCounter *counterTX;

    // QWidget interface
protected:
    void changeEvent(QEvent *event);
};

#endif // TABCOMSIMPLE_H
