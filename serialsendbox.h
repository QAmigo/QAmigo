#ifndef SERIALSENDBOX_H
#define SERIALSENDBOX_H

#include <QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
#include <QSerialPort>
#include <array>

class SerialSendBox : public QWidget
{
    Q_OBJECT
public:
    explicit SerialSendBox(QWidget *parent = nullptr, QSerialPort *port = nullptr);

signals:
    void errorSend(QString string);
    void addSendCount(int count);

private:
    char parseHex(QByteArray *array);
    int8_t toHex(char data);

private slots:
    void on_buttonSend_clicked();
    void onCheckBoxChanged();

private:
    QPushButton *buttonSend;
    QRadioButton *radioHex;
    QRadioButton *radioASC;
    QGroupBox *groupTranslateType;
    QPlainTextEdit *boxSend;
    QSerialPort *port;
};

#endif // SERIALSENDBOX_H
