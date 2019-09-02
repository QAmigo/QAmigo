#include "nameasker.h"
#include "ui_nameasker.h"

#include <QTimer>

NameAsker::NameAsker(QWidget *parent, QString name) :
    QDialog(parent),
    ui(new Ui::NameAsker)
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
    QTimer::singleShot(50, ui->lineEditName, &QLineEdit::selectAll);
    ui->lineEditName->setFocus();
}

NameAsker::~NameAsker()
{
    delete ui;
}

QString NameAsker::getName()
{
    return ui->lineEditName->text();
}
