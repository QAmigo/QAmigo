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
#include <QRegExpValidator>
#include <QLineEdit>
#include <QLabel>

#include "datavisualizationgraph.h"
#include "decodeditem.h"
#include "nameallocator.h"
#include "vartypeitem.h"

class TabAdvanced : public QWidget
{
    Q_OBJECT
public:
    explicit TabAdvanced(QWidget *parent = nullptr);
    ~TabAdvanced();

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
    void onDecodedItemClicked();
    void onButtonClearLogClicked();

private:
    void updateDecodeParameters();

private:
    QPushButton *buttonEnable;
    QLabel *labelHeader;
    QLineEdit *boxHeader;
    QLabel *labelType;
    QComboBox *comboType;
    QListWidget	*listProtocal;
    QGroupBox *groupEndianess;
    QRadioButton *radioLittle;
    QRadioButton *radioBig;
    QPushButton *buttonAdd;
    QPushButton *buttonDelete;
    QPushButton *buttonUp;
    QPushButton *buttonDown;

    QPlainTextEdit *boxLog;
    QPushButton *buttonClearLog;

    DataVisualizationGraph *graph;

    bool enabled;
    ENDIANESS endianess;

    QGridLayout *layoutLabels;
    uint8_t countLabels;
    QList<DecodedItem *> *listDecodedItems;

    NameAllocator *nameAllocator;
    QRegExpValidator *validatorHeader;
};

#endif // TABADVANCED_H
