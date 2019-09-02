#ifndef NAMEASKER_H
#define NAMEASKER_H

#include <QDialog>

namespace Ui {
class NameAsker;
}

class NameAsker : public QDialog
{
    Q_OBJECT

public:
    explicit NameAsker(QWidget *parent = nullptr, QString name = "", QString regexText = "");
    ~NameAsker();

    QString getName();

private:
    Ui::NameAsker *ui;
};

#endif // NAMEASKER_H
