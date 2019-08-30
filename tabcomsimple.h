#ifndef TABCOMSIMPLE_H
#define TABCOMSIMPLE_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QPlainTextEdit>
#include <QSerialPort>

class TabCOMSimple : public QWidget
{
    Q_OBJECT
public:
    explicit TabCOMSimple(QWidget *parent = nullptr,
                          QSerialPort *port = nullptr);

signals:
    void errorMessage(QString string);

public slots:
    void rawDataReady(const QByteArray &array);

private slots:
    void onButtonReceiveClearClicked();
    void sendError(QString string);

private:
    QPlainTextEdit *boxReceive;
    QRadioButton *radioReceiveHex;
    QRadioButton *radioReceiveASC;
    QPushButton *buttonReceiveClear;
};

#endif // TABCOMSIMPLE_H
