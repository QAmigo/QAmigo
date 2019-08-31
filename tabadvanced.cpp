#include "tabadvanced.h"
#include "vartypeitem.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

#include <sstream>
#include <iostream>

TabAdvanced::TabAdvanced(QWidget *parent) : QWidget(parent),
    buttonEnable(new QPushButton("Enable")),
    labelHeader(new QLabel("Frame Header")),
    boxHeader(new QLineEdit("AAAA")),
    labelType(new QLabel("Var Type")),
    comboType(new QComboBox()),
    listProtocal(new QListWidget()),
    checkCRC(new QCheckBox()),
    groupEndianess(new QGroupBox("Endianess")),
    radioLittle(new QRadioButton("Little")),
    radioBig(new QRadioButton("Big")),
    buttonAdd(new QPushButton("Add")),
    buttonDelete(new QPushButton("Delete")),
    buttonUp(new QPushButton("Up")),
    buttonDown(new QPushButton("Down")),
    boxLog(new QPlainTextEdit()),
    buttonClearLog(new QPushButton("Clear")),
    graph(new DataVisualizationGraph()),
    enabled(false),
    endianess(BIG),
    layoutLabels(new QGridLayout()),
    countLabels(0),
    listDecodedItems(new QList<DecodedItem *>()),
    nameAllocator(new NameAllocator())
{
    QGridLayout *layoutMain = new QGridLayout();
    setLayout(layoutMain);

    QHBoxLayout *layoutList = new QHBoxLayout();
    QVBoxLayout *layoutLog = new QVBoxLayout();

    layoutMain->addLayout(layoutList, 0, 0);
    layoutMain->addLayout(layoutLabels, 0, 1);
    layoutMain->addLayout(layoutLog, 1, 0);
    layoutMain->addWidget(graph, 1, 1);

    comboType->addItem("U8", VAR_TYPE::U8);
    comboType->addItem("I8", VAR_TYPE::I8);
    comboType->addItem("U16", VAR_TYPE::U16);
    comboType->addItem("I16", VAR_TYPE::I16);
    comboType->addItem("U32", VAR_TYPE::U32);
    comboType->addItem("I32", VAR_TYPE::I32);
    comboType->addItem("float", VAR_TYPE::FLOAT);
    comboType->addItem("double", VAR_TYPE::DOUBLE);

    layoutList->addWidget(listProtocal);
    QVBoxLayout *layoutListControls = new QVBoxLayout();
    layoutList->addLayout(layoutListControls);
    layoutListControls->addWidget(buttonEnable);
    layoutListControls->addWidget(labelHeader);
    layoutListControls->addWidget(boxHeader);
    boxHeader->setMaxLength(4);
    connect(boxHeader, &QLineEdit::textChanged, this, &TabAdvanced::onBoxHeaderTextChanged);
    layoutListControls->addWidget(labelType);
    layoutListControls->addWidget(comboType);
    layoutListControls->addWidget(groupEndianess);
    QVBoxLayout *layoutEndianess = new QVBoxLayout();
    groupEndianess->setLayout(layoutEndianess);
    layoutEndianess->addWidget(radioBig);
    layoutEndianess->addWidget(radioLittle);
    radioBig->setChecked(true);
    connect(radioLittle, &QRadioButton::clicked, this, &TabAdvanced::onRadioLittleBigClicked);
    connect(radioBig, &QRadioButton::clicked, this, &TabAdvanced::onRadioLittleBigClicked);
    layoutListControls->addWidget(buttonAdd);
    layoutListControls->addWidget(buttonDelete);
    layoutListControls->addWidget(buttonUp);
    layoutListControls->addWidget(buttonDown);
    layoutListControls->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(buttonAdd, &QPushButton::clicked, this, &TabAdvanced::onButtonAddClicked);
    connect(buttonDelete, &QPushButton::clicked, this, &TabAdvanced::onButtonDeleteClicked);
    connect(buttonUp, &QPushButton::clicked, this, &TabAdvanced::onButtonUpClicked);
    connect(buttonDown, &QPushButton::clicked, this, &TabAdvanced::onButtonDownClicked);
    connect(buttonEnable, &QPushButton::clicked, this, &TabAdvanced::onButtonEnableClicked);

    layoutList->setStretchFactor(listProtocal, 4);
    layoutList->setStretchFactor(layoutListControls, 1);

    layoutLog->addWidget(boxLog);
    QHBoxLayout *layoutLogControls = new QHBoxLayout();
    layoutLog->addLayout(layoutLogControls);
    layoutLogControls->addWidget(buttonClearLog);
}

void TabAdvanced::frameDataReady(QByteArray array)
{
    QString bufferShow;
    if (enabled) {
        int count = 0;
        QList<double> list;
        for (int i = 0; i < listProtocal->count(); i++) {
            VarTypeItem *item = static_cast<VarTypeItem *>(listProtocal->item(i));
            item->setBufferValue(array.mid(count, item->getSize()));
            count += item->getSize();
            double val = item->getDouble(endianess);
            bufferShow.append(QString().sprintf("%4.2lf ", val));
            if ((*listDecodedItems)[i]->checkShow->isChecked())
                list.append(val);
        }
        if (list.count() != 0)
            graph->appendData(list);
        if (bufferShow.count() != 0)
            boxLog->appendPlainText(bufferShow);
    }
}

void TabAdvanced::onButtonAddClicked()
{
    if (listProtocal->count() < 25) {
        VarTypeItem *typeItem = new VarTypeItem(comboType->currentText(), static_cast<VAR_TYPE>(comboType->currentData().toInt()), nameAllocator->allocateName());
        listProtocal->addItem(typeItem);
        listProtocal->setCurrentRow(listProtocal->count() - 1);
        updateDecodeParameters();
        //Max 25 labels.
        DecodedItem *decodedItem = new DecodedItem(this, typeItem->getName());
        connect(decodedItem->checkShow, &QCheckBox::clicked, this, &TabAdvanced::onDecodedItemClicked);
        layoutLabels->addWidget(decodedItem, countLabels / 5, countLabels % 5);
        listDecodedItems->append(decodedItem);
        countLabels++;
    }
}

void TabAdvanced::onButtonDeleteClicked()
{
    layoutLabels->removeWidget((*listDecodedItems)[listProtocal->currentRow()]);
    delete listDecodedItems->takeAt(listProtocal->currentRow());
    //Attention: current row changed after the following line.
    VarTypeItem *item = static_cast<VarTypeItem *>(listProtocal->takeItem(listProtocal->currentRow()));
    nameAllocator->freeName(item->getName());
    updateDecodeParameters();
}

void TabAdvanced::onButtonUpClicked()
{
    int currentRow = listProtocal->currentRow();
    if (currentRow != 0) {
        listProtocal->insertItem(currentRow - 1, listProtocal->takeItem(currentRow));
        listProtocal->setCurrentRow(currentRow - 1);
    }
    updateDecodeParameters();
}

void TabAdvanced::onButtonDownClicked()
{
    int currentRow = listProtocal->currentRow();
    if (listProtocal->count() != currentRow + 1) {
        listProtocal->insertItem(currentRow + 1, listProtocal->takeItem(currentRow));
        listProtocal->setCurrentRow(currentRow + 1);
    }
    updateDecodeParameters();
}

void TabAdvanced::onButtonEnableClicked()
{
    if (enabled == false) {
        buttonEnable->setText("Disable");
        enabled = true;
        updateDecodeParameters();
    } else {
        buttonEnable->setText("Enable");
        enabled = false;
    }
}

void TabAdvanced::onRadioLittleBigClicked()
{
    if (radioLittle->isChecked())
        endianess = LITTLE;
    else
        endianess = BIG;
}

void TabAdvanced::onBoxHeaderTextChanged()
{
    updateDecodeParameters();
}

void TabAdvanced::onDecodedItemClicked()
{
    for (DecodedItem *item : *listDecodedItems) {
        if (item->isCheckChanged()) {
            item->clearState();
            if (item->checkShow->isChecked()) {
                graph->createSeries(item->getName());
            } else {

            }
            return;
        }
    }
}

void TabAdvanced::updateDecodeParameters()
{
    std::stringstream ss;
    uint16_t x;
    QByteArray header;
    ss << std::hex << boxHeader->text().toUtf8().constData();
    ss >> x;
    header.append(static_cast<char>(x >> 8));
    header.append(static_cast<char>(x));
    emit sendDecodeParameters(header, *listProtocal);
}
