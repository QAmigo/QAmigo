#include "sendbox.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QMessageBox>

SendBox::SendBox(QWidget *parent) : QWidget(parent),
    buttonSend(new QPushButton(tr("Send"), this)),
    radioHex(new QRadioButton(tr("Hex"), this)),
    radioASC(new QRadioButton(tr("ASC"), this)),
    boxSend(new QPlainTextEdit(this)),
    checkTimer(new QCheckBox()),
    labelTimer(new QLabel(tr("Repeat Send Every"))),
    spinTimer(new QSpinBox()),
    labelMs(new QLabel(tr("ms"))),
    timer(new QBasicTimer())
{
    radioASC->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(boxSend);

    QHBoxLayout *layoutControls = new QHBoxLayout();
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

    connect(buttonSend, &QPushButton::clicked, this, &SendBox::on_buttonSend_clicked);
    connect(radioASC, &QRadioButton::clicked, this, &SendBox::onCheckBoxChanged);
    connect(radioHex, &QRadioButton::clicked, this, &SendBox::onCheckBoxChanged);
    connect(checkTimer, &QCheckBox::stateChanged, this, &SendBox::onCheckTimerStateChanged);
    connect(spinTimer, QOverload<int>::of(&QSpinBox::valueChanged), this, &SendBox::onSpinTimerValueChanged);
}

SendBox::~SendBox()
{
    delete timer;
}

void SendBox::bindIODevice(QIODevice *ioDevice)
{
    this->ioDevice = ioDevice;
}

int8_t SendBox::toHex(char data)
{
    if (data <= '9' && data >= '0')
        return data - '0';
    if (data <= 'f' && data >= 'a')
        return data - 'a' + 10;

    return -1;
}

char SendBox::parseHex(QByteArray *array)
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

void SendBox::on_buttonSend_clicked()
{
    if (!ioDevice)
        return;
    if (ioDevice->isOpen()) {
        if (radioASC->isChecked()) {
            QString text = boxSend->toPlainText();
            ioDevice->write(text.toUtf8());
            emit addSendCount(text.count());
        } else {
            QByteArray bufferSend;
            QByteArray *bufferInput = new QByteArray(boxSend->toPlainText().remove(' ').toLower().toUtf8());
            while (bufferInput->size() != 0) {
                char hex = parseHex(bufferInput);
                bufferSend.append(hex);
            }
            if (bufferSend.size() != 0) {
                ioDevice->write(bufferSend);
                emit addSendCount(bufferSend.count());
            }
        }
    }
}

void SendBox::onCheckBoxChanged()
{
    boxSend->setPlainText("");
}

void SendBox::onCheckTimerStateChanged()
{
    if (checkTimer->checkState() == Qt::CheckState::Checked)
        timer->start(spinTimer->value(), this);
    else
        timer->stop();
}

void SendBox::onSpinTimerValueChanged(int i)
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

void SendBox::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer->timerId())
        on_buttonSend_clicked();
    else
        QWidget::timerEvent(event);
}

void SendBox::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QWidget::changeEvent(event);
}

void SendBox::retranslateUi()
{
    buttonSend->setText(QCoreApplication::translate("SendBox", "Send"));
    radioHex->setText(QCoreApplication::translate("SendBox", "Hex"));
    radioASC->setText(QCoreApplication::translate("SendBox", "ASC"));
    labelTimer->setText(QCoreApplication::translate("SendBox", "Repeat Send Every"));
    labelMs->setText(QCoreApplication::translate("SendBox", "ms"));
}
