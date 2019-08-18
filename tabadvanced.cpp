#include "tabadvanced.h"
#include "vartypeitem.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

TabAdvanced::TabAdvanced(QWidget *parent) : QWidget(parent),
    buttonEnable(new QPushButton("Enable")),
    labelHeader(new QLabel("Frame Header")),
    boxHeader(new QLineEdit()),
    labelType(new QLabel("Var Type")),
    comboType(new QComboBox()),
    listProtocal(new QListWidget()),
    checkCRC(new QCheckBox()),
    buttonAdd(new QPushButton("Add")),
    buttonDelete(new QPushButton("Delete")),
    buttonUp(new QPushButton("Up")),
    buttonDown(new QPushButton("Down")),
    boxLog(new QPlainTextEdit()),
    buttonClearLog(new QPushButton("Clear")),
    graph(new QGraphicsView()),
    buttonClearGraph(new QPushButton("Clear"))
{
    QGridLayout *layoutMain = new QGridLayout();
    setLayout(layoutMain);

    QHBoxLayout *layoutList = new QHBoxLayout();
    QGridLayout *layoutLabels = new QGridLayout();
    QVBoxLayout *layoutLog = new QVBoxLayout();
    QVBoxLayout *layoutGraph = new QVBoxLayout();

    layoutMain->addLayout(layoutList, 0, 0);
    layoutMain->addLayout(layoutLabels, 0, 1);
    layoutMain->addLayout(layoutLog, 1, 0);
    layoutMain->addLayout(layoutGraph, 1, 1);

    comboType->addItem("U8", VAR_TYPE::U8);
    comboType->addItem("I8", VAR_TYPE::I8);
    comboType->addItem("U16", VAR_TYPE::U16);
    comboType->addItem("I16", VAR_TYPE::I16);
    comboType->addItem("U32", VAR_TYPE::U32);
    comboType->addItem("I32", VAR_TYPE::I32);
    comboType->addItem("U64", VAR_TYPE::U64);
    comboType->addItem("I64", VAR_TYPE::I64);
    comboType->addItem("float", VAR_TYPE::FLOAT);
    comboType->addItem("double", VAR_TYPE::DOUBLE);

    layoutList->addWidget(listProtocal);
    QVBoxLayout *layoutListControls = new QVBoxLayout();
    layoutList->addLayout(layoutListControls);
    layoutListControls->addWidget(buttonEnable);
    layoutListControls->addWidget(labelHeader);
    layoutListControls->addWidget(boxHeader);
    layoutListControls->addWidget(labelType);
    layoutListControls->addWidget(comboType);
    layoutListControls->addWidget(buttonAdd);
    layoutListControls->addWidget(buttonDelete);
    layoutListControls->addWidget(buttonUp);
    layoutListControls->addWidget(buttonDown);
    layoutListControls->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(buttonAdd, &QPushButton::clicked, this, &TabAdvanced::onButtonAddClicked);
    connect(buttonDelete, &QPushButton::clicked, this, &TabAdvanced::onButtonDeleteClicked);
    connect(buttonUp, &QPushButton::clicked, this, &TabAdvanced::onButtonUpClicked);
    connect(buttonDown, &QPushButton::clicked, this, &TabAdvanced::onButtonDownClicked);

    layoutList->setStretchFactor(listProtocal, 4);
    layoutList->setStretchFactor(layoutListControls, 1);

    layoutLog->addWidget(boxLog);
    QHBoxLayout *layoutLogControls = new QHBoxLayout();
    layoutLog->addLayout(layoutLogControls);
    layoutLogControls->addWidget(buttonClearLog);

    layoutGraph->addWidget(graph);
    QHBoxLayout *layoutGraphControls = new QHBoxLayout();
    layoutGraph->addLayout(layoutGraphControls);
    layoutGraphControls->addWidget(buttonClearGraph);
}

void TabAdvanced::onButtonAddClicked()
{
    listProtocal->addItem(new VarTypeItem(comboType->currentText(), static_cast<VAR_TYPE>(comboType->currentData().toInt())));
}

void TabAdvanced::onButtonDeleteClicked()
{
    listProtocal->currentRow();
}

void TabAdvanced::onButtonUpClicked()
{
    int currentRow = listProtocal->currentRow();
    if (currentRow != 0) {
        listProtocal->insertItem(currentRow - 1, listProtocal->takeItem(currentRow));
        listProtocal->setCurrentRow(currentRow - 1);
    }
}

void TabAdvanced::onButtonDownClicked()
{
    int currentRow = listProtocal->currentRow();
    if (listProtocal->count() != currentRow + 1) {
        listProtocal->insertItem(currentRow + 1, listProtocal->takeItem(currentRow));
        listProtocal->setCurrentRow(currentRow + 1);
    }
}
