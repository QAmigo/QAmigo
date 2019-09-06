#include "textinput.h"
#include "ui_textinput.h"

#include <QPushButton>
#include <QRegExpValidator>
#include <QTimer>

TextInput::TextInput(QWidget *parent, QString name, QString regexText) :
    QDialog(parent),
    ui(new Ui::TextInput)
{
    ui->setupUi(this);

    ui->lineEditName->setText(name);

    /*
     * According to a stack overflow anwser, mousePressEvent will be
     * triggered just after focusInEvent, so all the selected text
     * will become unselcted since it thinks someone clicked.
     * This will add some delay to make selectAll operation happens
     * after the mousePressEvent.
     */
    if (regexText != "")
        ui->lineEditName->setValidator(new QRegExpValidator(QRegExp(regexText)));
    QTimer::singleShot(50, ui->lineEditName, &QLineEdit::selectAll);
    ui->lineEditName->setFocus();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

TextInput::~TextInput()
{
    delete ui;
}

QString TextInput::getText()
{
    return ui->lineEditName->text();
}

void TextInput::retranslateUi()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QCoreApplication::translate("TextInput", "OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QCoreApplication::translate("TextInput", "Cancel"));
}

void TextInput::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QDialog::changeEvent(event);
}
