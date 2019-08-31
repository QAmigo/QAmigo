#ifndef DECODEDITEM_H
#define DECODEDITEM_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

class DecodedItem : public QWidget
{
    Q_OBJECT
public:
    explicit DecodedItem(QWidget *parent = nullptr, const QString &name = "");

    bool isCheckChanged();
    void clearState();
    QString getName();

signals:

public slots:
    void onCheckBoxClicked();

public:
    QCheckBox *checkShow;

private:
    QLabel *labelName;	//Custom name?
    bool stateChanged;
};

#endif // DECODEDITEM_H
