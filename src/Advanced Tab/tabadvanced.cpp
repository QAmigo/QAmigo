#include "nameasker.h"
#include "tabadvanced.h"
#include "vartypeitem.h"

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
    groupEndianess(new QGroupBox("Endianess")),
    radioLittle(new QRadioButton("Little")),
    radioBig(new QRadioButton("Big")),
    mutexAdd(new QMutex()),
    buttonAdd(new QPushButton("Add")),
    buttonDelete(new QPushButton("Delete")),
    buttonUp(new QPushButton("Up")),
    buttonDown(new QPushButton("Down")),
    boxLog(new QPlainTextEdit()),
    buttonClearLog(new QPushButton("Clear")),
    graph(new DataVisualizationGraph()),
    enabled(false),
    endianess(BIG),
    nameAllocator(new NameAllocator()),
    validatorHeader(new QRegExpValidator(QRegExp("[0-9a-fA-F]{1,4}")))
{
    QVBoxLayout *layoutMain = new QVBoxLayout();
    setLayout(layoutMain);

    QHBoxLayout *layoutListLog = new QHBoxLayout();
    layoutMain->addLayout(layoutListLog);

    QHBoxLayout *layoutList = new QHBoxLayout();
    QVBoxLayout *layoutLog = new QVBoxLayout();

    layoutListLog->addLayout(layoutList);
    layoutListLog->addLayout(layoutLog);
    layoutMain->addWidget(graph);

    comboType->addItem("U8", VAR_TYPE::U8);
    comboType->addItem("I8", VAR_TYPE::I8);
    comboType->addItem("U16", VAR_TYPE::U16);
    comboType->addItem("I16", VAR_TYPE::I16);
    comboType->addItem("U32", VAR_TYPE::U32);
    comboType->addItem("I32", VAR_TYPE::I32);
    comboType->addItem("float", VAR_TYPE::Float);
    comboType->addItem("double", VAR_TYPE::Double);

    layoutList->addWidget(listProtocal);

    QVBoxLayout *layoutListControls = new QVBoxLayout();
    layoutList->addLayout(layoutListControls);
    layoutListControls->addWidget(buttonEnable);

    layoutListControls->addWidget(labelHeader);

    //QLineEdit boxHeader.
    layoutListControls->addWidget(boxHeader);
    boxHeader->setValidator(validatorHeader);
    connect(boxHeader, &QLineEdit::textChanged, this, &TabAdvanced::onBoxHeaderTextChanged);

    layoutListControls->addWidget(groupEndianess);
    QVBoxLayout *layoutEndianess = new QVBoxLayout();
    groupEndianess->setLayout(layoutEndianess);

    layoutListControls->addWidget(labelType);
    layoutListControls->addWidget(comboType);
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
    connect(listProtocal, &QListWidget::itemDoubleClicked, this, &TabAdvanced::onlistProtocalsDoubleClicked);

    layoutList->setStretchFactor(layoutListControls, 1);

    layoutLog->addWidget(boxLog);
    QHBoxLayout *layoutLogControls = new QHBoxLayout();
    layoutLog->addLayout(layoutLogControls);
    layoutLogControls->addWidget(buttonClearLog);
    connect(buttonClearLog, &QPushButton::clicked, this, &TabAdvanced::onButtonClearLogClicked);
}

TabAdvanced::~TabAdvanced()
{
    delete nameAllocator;
    delete validatorHeader;
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
            if (item->getSeries() != nullptr)
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
    if (mutexAdd->tryLock()) {
        QString name = nameAllocator->allocateName();
        NameAsker *ask = new NameAsker(this, name);
        ask->show();
        if (ask->exec() == QDialog::Accepted) {
            if (ask->getName().compare(name) != 0) {
                nameAllocator->freeName(name);
                name = ask->getName();
            }

            if (name.count() != 0) {
                //If the name is already used.
                for (int i = 0; i < listProtocal->count(); i++) {
                    VarTypeItem *item = static_cast<VarTypeItem *>(listProtocal->itemAt(i, 0));
                    if (item->getName().compare(name) == 0) {
                        QMessageBox::warning(this, "Name used.", "This name has already been used.");
                        mutexAdd->unlock();
                        return;
                    }
                }
                VarTypeItem *typeItem = new VarTypeItem(comboType->currentText(), static_cast<VAR_TYPE>(comboType->currentData().toInt()), name);
                listProtocal->addItem(typeItem);
                listProtocal->setCurrentRow(listProtocal->count() - 1);
                updateDecodeParameters();
            }
        } else {
            nameAllocator->freeName(name);
        }
        mutexAdd->unlock();
    }
}

void TabAdvanced::onButtonDeleteClicked()
{
    VarTypeItem *item = static_cast<VarTypeItem *>(listProtocal->takeItem(listProtocal->currentRow()));
    nameAllocator->freeName(item->getName());
    updateDecodeParameters();
}

void TabAdvanced::onButtonClearLogClicked()
{
    boxLog->clear();
}

void TabAdvanced::onlistProtocalsDoubleClicked(QListWidgetItem *item)
{
    VarTypeItem *varTypeItem = static_cast<VarTypeItem *>(item);

    if (varTypeItem->getSeries() == nullptr) {
        varTypeItem->setSeries(graph->createSeries(varTypeItem->getName()));
        varTypeItem->setBackground(Qt::cyan);
    } else {
        graph->removeSeries(varTypeItem->getSeries());
        varTypeItem->setSeries(nullptr);
        varTypeItem->setBackground(Qt::white);
    }
    item->setSelected(false);
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
