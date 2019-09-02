#ifndef NAMEASKER_H
#define NAMEASKER_H

#include <QDialog>

namespace Ui {
class TextInput;
}

class TextInput : public QDialog
{
    Q_OBJECT

public:
    explicit TextInput(QWidget *parent = nullptr, QString name = "", QString regexText = "");
    ~TextInput();

    QString getText();

private:
    Ui::TextInput *ui;
};

#endif // NAMEASKER_H
