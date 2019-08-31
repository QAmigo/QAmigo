#ifndef TABADVANCED_H
#define TABADVANCED_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
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
    void sendDecodeParameters(const QByteArray header, const QListWidget &types);

public slots:
    void frameDataReady(QByteArray array);

private slots:
    void onButtonAddClicked();
    void onButtonDeleteClicked();
    void onButtonUpClicked();
    void onButtonDownClicked();
    void onButtonEnableClicked();
    void onRadioLittleBigClicked();
    void onBoxHeaderTextChanged();

private:
    void updateDecodeParameters();

private:
    QPushButton *buttonEnable;
    QLabel *labelHeader;
    QLineEdit *boxHeader;
    QLabel *labelType;
    QComboBox *comboType;
    QListWidget	*listProtocal;
    QCheckBox *checkCRC;
    QGroupBox *groupEndianess;
    QRadioButton *radioLittle;
    QRadioButton *radioBig;
    QPushButton *buttonAdd;
    QPushButton *buttonDelete;
    QPushButton *buttonUp;
    QPushButton *buttonDown;

    QPlainTextEdit *boxLog;
    QPushButton *buttonClearLog;

    QGraphicsView *graph;
    QPushButton *buttonClearGraph;

    bool enabled;
    ENDIANESS endianess;
};

#endif // TABADVANCED_H
