#include "serialsendbox.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QMessageBox>

SerialSendBox::SerialSendBox(QWidget *parent, QSerialPort *port) : QWidget(parent),
    buttonSend(new QPushButton(tr("Send"), this)),
    radioHex(new QRadioButton(tr("Hex"), this)),
    radioASC(new QRadioButton(tr("ASC"), this)),
    boxSend(new QPlainTextEdit(this)),
    port(port),
    checkTimer(new QCheckBox()),
    labelTimer(new QLabel(tr("Repeat Send Every"))),
    spinTimer(new QSpinBox()),
    labelMs(new QLabel(tr("ms"))),
    timer(new QBasicTimer())
{
    radioASC->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(boxSend);

    QHBoxLayout *layoutControls = new QHBoxLayout(this);
    layoutControls->addWidget(radioASC);
    layoutControls->addWidget(radioHex);
    layoutControls->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutControls->addWidget(checkTimer);
    layoutControls->addWidget(labelTimer);
    layoutControls->addWidget(spinTimer);
    spinTimer->setMinimum(1);
    spinTimer->setMaximum(99999);
    spinTimer->setValue(1000);
    layoutControls->addWidget(labelMs);
    layoutControls->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutControls->addWidget(buttonSend);
    layout->addLayout(layoutControls);
    setLayout(layout);

    connect(buttonSend, &QPushButton::clicked, this, &SerialSendBox::on_buttonSend_clicked);
    connect(radioASC, &QRadioButton::clicked, this, &SerialSendBox::onCheckBoxChanged);
    connect(radioHex, &QRadioButton::clicked, this, &SerialSendBox::onCheckBoxChanged);
    connect(checkTimer, &QCheckBox::stateChanged, this, &SerialSendBox::onCheckTimerStateChanged);
    connect(spinTimer, QOverload<int>::of(&QSpinBox::valueChanged), this, &SerialSendBox::onSpinTimerValueChanged);
}

SerialSendBox::~SerialSendBox()
{
    delete timer;
}

int8_t SerialSendBox::toHex(char data)
{
    if (data <= '9' && data >= '0')
        return data - '0';
    if (data <= 'f' && data >= 'a')
        return data - 'a' + 10;

    return -1;
}

char SerialSendBox::parseHex(QByteArray *array)
{
    if (array->size() >= 2) {
        QByteArray hexArray = array->left(2);
        array->remove(0, 2);
        int8_t a, b;
        a = toHex(hexArray[0]);
        b = toHex(hexArray[1]);
        if (a >=0 && b >=0) {
            return a * 16 + b;
        }
    }

    return 0;
}

void SerialSendBox::on_buttonSend_clicked()
{
    if (port->isOpen()) {
        if (radioASC->isChecked()) {
            QString text = boxSend->toPlainText();
            port->write(text.toUtf8());
            emit addSendCount(text.count());
        } else {
            QByteArray bufferSend;
            QByteArray *bufferInput = new QByteArray(boxSend->toPlainText().remove(' ').toLower().toUtf8());
            while (bufferInput->size() != 0) {
                char hex = parseHex(bufferInput);
                bufferSend.append(hex);
            }
            if (bufferSend.size() != 0) {
                port->write(bufferSend);
                emit addSendCount(bufferSend.count());
            }
        }
    }
}

void SerialSendBox::onCheckBoxChanged()
{
    boxSend->setPlainText("");
}

void SerialSendBox::onCheckTimerStateChanged()
{
    if (checkTimer->checkState() == Qt::CheckState::Checked)
        timer->start(spinTimer->value(), this);
    else
        timer->stop();
}

void SerialSendBox::onSpinTimerValueChanged(int i)
{
    /*
     * If value of spinbox is edited when repeat sending ebaled,
     * new value will be reloaded to the timer automatically.
     */
    if (checkTimer->checkState() == Qt::CheckState::Checked) {
        timer->stop();
        timer->start(i, this);
    }
}

void SerialSendBox::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer->timerId())
        on_buttonSend_clicked();
    else
        QWidget::timerEvent(event);
}

void SerialSendBox::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QWidget::changeEvent(event);
}

void SerialSendBox::retranslateUi()
{
    buttonSend->setText(QCoreApplication::translate("SerialSendBox", "Send"));
    radioHex->setText(QCoreApplication::translate("SerialSendBox", "Hex"));
    radioASC->setText(QCoreApplication::translate("SerialSendBox", "ASC"));
    labelTimer->setText(QCoreApplication::translate("SerialSendBox", "Repeat Send Every"));
    labelMs->setText(QCoreApplication::translate("SerialSendBox", "ms"));
}
