#ifndef SERIALSENDBOX_H
#define SERIALSENDBOX_H

#include <QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
#include <QSerialPort>

class SerialSendBox : public QWidget
{
    Q_OBJECT
public:
    explicit SerialSendBox(QWidget *parent = nullptr, QSerialPort *port = nullptr);

signals:

public slots:

    // QWidget interface
protected:

private slots:
    void on_buttonSend_clicked();

private:
    QPushButton *buttonSend;
    QRadioButton *radioHex;
    QRadioButton *radioASC;
    QGroupBox *groupTranslateType;
    QPlainTextEdit *boxSend;
    QSerialPort *port;
};

#endif // SERIALSENDBOX_H
