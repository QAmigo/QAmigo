#include "textinput.h"
#include "ui_textinput.h"

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
}

TextInput::~TextInput()
{
    delete ui;
}

QString TextInput::getText()
{
    return ui->lineEditName->text();
}
