#ifndef SENDBOX_H
#define SENDBOX_H

#include <QBasicTimer>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QSerialPort>

class SendBox : public QWidget
{
    Q_OBJECT
public:
    explicit SendBox(QWidget *parent);
    // Is this because I overrided timerEvent so a default destructor is generated?
    ~SendBox() override;

    void bindIODevice(QIODevice *ioDevice);

signals:
    void errorSend(QString string);
    void addSendCount(int count);

private:
    char parseHex(QByteArray *array);
    int8_t toHex(char data);
    void retranslateUi();

private slots:
    void on_buttonSend_clicked();
    void onCheckBoxChanged();
    void onCheckTimerStateChanged();
    void onSpinTimerValueChanged(int i);

private:
    QPushButton *buttonSend;
    QRadioButton *radioHex;
    QRadioButton *radioASC;
    QGroupBox *groupTranslateType;
    QPlainTextEdit *boxSend;
    QIODevice *ioDevice;
    QCheckBox *checkTimer;
    QLabel *labelTimer;
    QSpinBox *spinTimer;
    QLabel *labelMs;
    QBasicTimer *timer;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) override;

    // QWidget interface
protected:
    void changeEvent(QEvent *event) override;
};

#endif // SENDBOX_H
