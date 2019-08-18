#ifndef TABADVANCED_H
#define TABADVANCED_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QLineEdit>
#include <QLabel>

#include "vartypeitem.h"

class TabAdvanced : public QWidget
{
    Q_OBJECT
public:
    explicit TabAdvanced(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void onButtonAddClicked();
    void onButtonDeleteClicked();
    void onButtonUpClicked();
    void onButtonDownClicked();

private:
    QPushButton *buttonEnable;
    QLabel *labelHeader;
    QLineEdit *boxHeader;
    QLabel *labelType;
    QComboBox *comboType;
    QListWidget	*listProtocal;
    QCheckBox *checkCRC;
    QPushButton *buttonAdd;
    QPushButton *buttonDelete;
    QPushButton *buttonUp;
    QPushButton *buttonDown;

    QPlainTextEdit *boxLog;
    QPushButton *buttonClearLog;

    QGraphicsView *graph;
    QPushButton *buttonClearGraph;
};

#endif // TABADVANCED_H
