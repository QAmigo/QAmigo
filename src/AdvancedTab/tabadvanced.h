#ifndef TABADVANCED_H
#define TABADVANCED_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExpValidator>
#include <QTreeView>
#include <QLineEdit>
#include <QLabel>

#include "datavisualizationgraph.h"
#include "nameallocator.h"
#include "protocal.h"
#include "vartype.h"

class TabAdvanced : public QWidget
{
    Q_OBJECT
public:
    explicit TabAdvanced(QWidget *parent = nullptr);
    ~TabAdvanced();

    const QList<Protocal *> &getListProtocals();
    const ENDIANESS &getEndianess();

    void setAllowRunning(bool value);

signals:
    void onDecodeParametersUpdated(const QList<Protocal *> &listProtocals);

public slots:
    void frameDataReady(int id, QList<double> listValues);

private slots:
    void onButtonAddHeaderClicked();
    void onButtonAddDataClicked();
    void onButtonDeleteClicked();
    void onButtonUpClicked();
    void onButtonDownClicked();
    void onButtonEnableClicked();
    void onButtonLoadSettingsClicked();
    void onButtonSaveSettingsClicked();
    void onRadioLittleBigClicked();
    void onButtonClearLogClicked();
    void ontreeProtocalItemChanged(QStandardItem *item);
    void onGraphUpdate(int currentX);

private:
    void updateDecodeParameters();
    bool checkIfHeaderExists(QByteArray header);
    bool checkIfNameExists(QString name);
    void addHeader(QByteArray header);
    void addData(QModelIndex index, QString name, int indexType);
    void updateSelection(QModelIndex index);

private:
    QPushButton *buttonEnable;
    QPushButton *buttonLoad;
    QPushButton *buttonSave;
    QLabel *labelType;
    QComboBox *comboType;
    QTreeView *treeProtocals;
    QStandardItemModel *model;
    QItemSelectionModel *selectionModel;
    QGroupBox *groupEndianess;
    QRadioButton *radioLittle;
    QRadioButton *radioBig;
    QList<Protocal *> *listProtocals;

    QMutex *mutexAdd;
    QPushButton *buttonAddHeader;
    QPushButton *buttonAddData;
    QPushButton *buttonDelete;
    QPushButton *buttonUp;
    QPushButton *buttonDown;

    QPlainTextEdit *boxLog;
    QPushButton *buttonClearLog;

    DataVisualizationGraph *graph;

    bool enabled;
    ENDIANESS endianess;

    NameAllocator *nameAllocator;
    QRegExpValidator *validatorHeader;

    bool allowRunning;
};

#endif // TABADVANCED_H
