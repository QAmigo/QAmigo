#include "protocaldataitem.h"
#include "protocalheaderitem.h"
#include "tabadvanced.h"
#include "textinput.h"
#include "texttranslator.h"
#include "vartype.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStandardItemModel>

#include <sstream>
#include <iostream>

TabAdvanced::TabAdvanced(QWidget *parent) : QWidget(parent),
    buttonEnable(new QPushButton("Enable")),
    buttonLoad(new QPushButton("Load Settings")),
    buttonSave(new QPushButton("Save Settings")),
    labelType(new QLabel("Var Type")),
    comboType(new QComboBox()),
    treeProtocals(new QTreeView()),
    model(new QStandardItemModel()),
    groupEndianess(new QGroupBox("Endianess")),
    radioLittle(new QRadioButton("Little")),
    radioBig(new QRadioButton("Big")),
    listProtocals(new QList<Protocal *>()),
    mutexAdd(new QMutex()),
    buttonAddHeader(new QPushButton("Add Header")),
    buttonAddData(new QPushButton("Add Data")),
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
    connect(graph, &DataVisualizationGraph::onGraphUpdate, this, &TabAdvanced::onGraphUpdate);

    comboType->addItem("U8", VAR_TYPE::U8);
    comboType->addItem("I8", VAR_TYPE::I8);
    comboType->addItem("U16", VAR_TYPE::U16);
    comboType->addItem("I16", VAR_TYPE::I16);
    comboType->addItem("U32", VAR_TYPE::U32);
    comboType->addItem("I32", VAR_TYPE::I32);
    comboType->addItem("float", VAR_TYPE::Float);
    comboType->addItem("double", VAR_TYPE::Double);

    layoutList->addWidget(treeProtocals);
    layoutList->setStretchFactor(treeProtocals, 4);
    treeProtocals->setModel(model);
    connect(model, &QStandardItemModel::itemChanged, this, &TabAdvanced::ontreeProtocalItemChanged);
    selectionModel = treeProtocals->selectionModel();

    model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Data") << QStringLiteral("Name"));
    QVBoxLayout *layoutListControls = new QVBoxLayout();
    layoutList->addLayout(layoutListControls);
    layoutListControls->addWidget(buttonEnable);
    layoutListControls->addWidget(buttonLoad);
    layoutListControls->addWidget(buttonSave);

    connect(buttonLoad, &QPushButton::clicked, this, &TabAdvanced::onButtonLoadSettingsClicked);
    connect(buttonSave, &QPushButton::clicked, this, &TabAdvanced::onButtonSaveSettingsClicked);

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
    layoutListControls->addWidget(buttonAddHeader);
    layoutListControls->addWidget(buttonAddData);
    layoutListControls->addWidget(buttonDelete);
    layoutListControls->addWidget(buttonUp);
    layoutListControls->addWidget(buttonDown);
    layoutListControls->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(buttonAddHeader, &QPushButton::clicked, this, &TabAdvanced::onButtonAddHeaderClicked);
    connect(buttonAddData, &QPushButton::clicked, this, &TabAdvanced::onButtonAddDataClicked);
    connect(buttonDelete, &QPushButton::clicked, this, &TabAdvanced::onButtonDeleteClicked);
    connect(buttonUp, &QPushButton::clicked, this, &TabAdvanced::onButtonUpClicked);
    connect(buttonDown, &QPushButton::clicked, this, &TabAdvanced::onButtonDownClicked);
    connect(buttonEnable, &QPushButton::clicked, this, &TabAdvanced::onButtonEnableClicked);

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
    qDeleteAll(*listProtocals);
}

void TabAdvanced::frameDataReady(int id, QList<double> listValues)
{
    QString bufferShow = QString().sprintf("[%d] \t", id);
    for (int i = 0; i < listValues.count(); i++) {
        bufferShow.append(QString().sprintf("%.2lf ", listValues.at(i)));
        ProtocalDataItem *item = static_cast<ProtocalDataItem *>(model->item(id)->child(i));
        if (item->checkState() == Qt::CheckState::Checked) {
            item->setCurrentValue(listValues.at(i));
        }
    }
    boxLog->appendPlainText(bufferShow);
}

bool TabAdvanced::checkIfHeaderExists(QByteArray header)
{
    for (int i = 0; i < listProtocals->count(); i++) {
        ProtocalHeaderItem *item = static_cast<ProtocalHeaderItem *>(model->item(i));
        if (item->getHeader().compare(header) == 0)
            return true;
    }
    return false;
}

bool TabAdvanced::checkIfNameExists(QString name)
{
    for (int i = 0; i < listProtocals->count(); i++) {
        ProtocalHeaderItem *headerItem = static_cast<ProtocalHeaderItem *>(model->item(i));
        for (int j = 0; j < headerItem->rowCount(); j++) {
            ProtocalDataItem *dataItem = static_cast<ProtocalDataItem *>(headerItem->child(j));
            if (dataItem->getName().compare(name) == 0)
                return true;
        }
    }
    return false;
}

void TabAdvanced::setAllowRunning(bool value)
{
    allowRunning = value;
    graph->setAllowRunning(allowRunning);
}

void TabAdvanced::addHeader(QByteArray header)
{
    QList<QStandardItem *> list;
    list.append(new ProtocalHeaderItem(header));
    list.append(new QStandardItem());

    model->appendRow(list);
    int count = model->rowCount();
    QModelIndex indexLast = model->index(count - 1, 0, QModelIndex());
    updateSelection(indexLast);
}

void TabAdvanced::onButtonAddHeaderClicked()
{
    TextInput *input = new TextInput(this, "AAAA", "[0-9a-fA-F]+");
    input->show();
    if (input->exec() == QDialog::Accepted) {
        TextTranslator translator = TextTranslator(input->getText());
        QByteArray header = translator.toHex();
        if (checkIfHeaderExists(header)) {
            QMessageBox::warning(this, "Error", "Header already exists.");
        } else {
            addHeader(header);
        }
    }
}

//index of combo is corresponding to the index of type declared in VAR_TYPE.
void TabAdvanced::addData(QModelIndex index, QString name, int indexType)
{
    QList<QStandardItem *> list;
    VAR_TYPE type = static_cast<VAR_TYPE>(comboType->itemData(indexType).toInt());
    list.append(new ProtocalDataItem(type, comboType->itemText(indexType)));
    list.append(new QStandardItem(name));
    QModelIndex indexLast;
    if (index.parent().isValid()) {
        model->itemFromIndex(index)->parent()->appendRow(list);
        //Calculate the index of the last element.
        int count = model->itemFromIndex(index)->parent()->rowCount();
        indexLast = index.siblingAtRow(count - 1);
    }
    else {
        model->itemFromIndex(index.siblingAtColumn(0))->appendRow(list);
        int count = model->itemFromIndex(index)->rowCount();
        indexLast = model->index(count - 1, 0, index);
    }
    updateSelection(indexLast);
}

void TabAdvanced::updateSelection(QModelIndex index)
{
    selectionModel->select(index, QItemSelectionModel::SelectionFlag::ClearAndSelect |
                   QItemSelectionModel::SelectionFlag::Rows);
    selectionModel->setCurrentIndex(index, QItemSelectionModel::SelectionFlag::Current);
    updateDecodeParameters();
}

void TabAdvanced::clearHead(QModelIndex index)
{
    for (int i = 0; i < model->itemFromIndex(index)->rowCount(); i++) {
        nameAllocator->freeName(model->itemFromIndex(index)->child(i, 1)->text());
    }
    model->removeRow(index.row());
}

void TabAdvanced::clearTree()
{
    int count = model->rowCount();
    //NOTICE: model->rowCount() is changing wth deletion.
    for (int i = 0; i < count; i++)
        clearHead(model->index(0, 0));
}

void TabAdvanced::onButtonAddDataClicked()
{
    QModelIndex index = selectionModel->currentIndex();
    if (index.isValid()) {
        QString allocatedName = nameAllocator->allocateName();
        TextInput *input = new TextInput(this, allocatedName);
        input->show();
        if (input->exec() == QDialog::Accepted) {
            QString name = input->getText();
            if (name.compare(allocatedName) != 0) {
                nameAllocator->freeName(allocatedName);
            }
            if (checkIfNameExists(name)) {
                QMessageBox::warning(this, "Error", "Name already exists.");
                return;
            }
            addData(index, name, comboType->currentIndex());
        } else
            nameAllocator->freeName(allocatedName);
    }
}

void TabAdvanced::onButtonDeleteClicked()
{
    QModelIndex index = treeProtocals->selectionModel()->currentIndex();
    if (index.isValid()) {
        if (index.parent().isValid()) {
            QStandardItem *item = (model->itemFromIndex(index.siblingAtColumn(1)));
            nameAllocator->freeName(item->text());
            model->removeRow(index.row(), index.parent());
        }
        else
            clearHead(index);
        updateDecodeParameters();
    }
}

void TabAdvanced::onButtonClearLogClicked()
{
    boxLog->clear();
}

void TabAdvanced::ontreeProtocalItemChanged(QStandardItem *item)
{
    //Assume only checkbox will change now.
    ProtocalDataItem *dataItem = static_cast<ProtocalDataItem *>(item);
    if (item->checkState() == Qt::CheckState::Checked) {
        QString name = model->itemFromIndex(dataItem->index().siblingAtColumn(1))->text();
        dataItem->setSeries(graph->createSeries(name));
    } else {
        graph->removeSeries(dataItem->getSeries());
        dataItem->clearSeries();
    }
}

void TabAdvanced::onGraphUpdate(int currentX)
{
    for (int i = 0; i < model->rowCount(); i++) {
        ProtocalHeaderItem *headerItem = static_cast<ProtocalHeaderItem *>(model->item(i));
        for (int j = 0; j < headerItem->rowCount(); j++) {
            ProtocalDataItem *dataItem = static_cast<ProtocalDataItem *>(headerItem->child(j));
            dataItem->onGraphUpdate(currentX);
        }
    }
}

void TabAdvanced::onButtonUpClicked()
{
    QModelIndex index = selectionModel->currentIndex();
    if (index.isValid()) {
        if (index.siblingAtRow(0) != index) {
            if (!index.parent().isValid()) {
                model->insertRow(index.row() - 1, model->takeRow(index.row()));
            } else {
                QStandardItem *item = model->itemFromIndex(index);
                item->parent()->insertRow(index.row() - 1, item->parent()->takeRow(index.row()));
            }
            updateSelection(index.siblingAtRow(index.row() - 1));
        }
    }
}

void TabAdvanced::onButtonDownClicked()
{
    QModelIndex index = selectionModel->currentIndex();
    int oldRow = index.row();
    if (index.isValid()) {
        if (!index.parent().isValid()) {	//If it's the last one.
            if (index.row() == model->rowCount() - 1)
                return;
            model->insertRow(index.row() + 1, model->takeRow(index.row()));
        } else {
            if (index.row() == model->itemFromIndex(index)->parent()->rowCount() - 1)
                return;

            QStandardItem *item = model->itemFromIndex(index);
            item->parent()->insertRow(index.row() + 1, item->parent()->takeRow(index.row()));
        }
        updateSelection(index.siblingAtRow(oldRow + 1));
    }
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

void TabAdvanced::onButtonLoadSettingsClicked()
{
    QString folderString = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/QSerial Socket Amigo";
    QFileInfo folder(folderString);
    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open Protocal Settings"),
                folderString,
                tr("JSON (*.json)"));
    if (fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "File open failed.");
            return;
        }

        clearTree();
        QByteArray saveData = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject object = loadDoc.object();
        endianess = static_cast<ENDIANESS>(object["endianess"].toInt());
        if (endianess)
            radioLittle->setChecked(true);
        else
            radioBig->setChecked(true);
        for (int i = 0; i < object.keys().count() - 1; i++) {
            QJsonObject frameObject = object[QString(i)].toObject();
            TextTranslator translator(frameObject["header"].toString());
            addHeader(translator.toHex());
            QJsonArray types = frameObject["types"].toArray();
            for (int j = 0; j < types.count(); j++) {
                QJsonObject type = types.at(j).toObject();
                QModelIndex index = model->index(model->rowCount() - 1, 0);
                QString name = type["name"].toString();
                nameAllocator->setNameUsed(name);
                addData(index, name, type["type"].toInt());
            }
        }
        file.close();
    }
}

void TabAdvanced::onButtonSaveSettingsClicked()
{
    QString folderString = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/QSerial Socket Amigo";
    QFileInfo folder(folderString);
    QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Protocal Settings"),
                folderString,
                tr("JSON (*.json)"));
    if (fileName.isEmpty()) {
        return;
    } else {
        if (fileName.right(5).toLower().compare(".json") != 0)
            fileName.append(".json");

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Error", "File open failed.");
            return;
        }

        QJsonObject object = QJsonObject();
        for (int i = 0; i < model->rowCount(); i++) {
            ProtocalHeaderItem *header = static_cast<ProtocalHeaderItem *>(model->item(i));
            QJsonObject frameObject = QJsonObject();
            frameObject["header"] = QString(header->getHeader().toHex()).toUpper();
            QJsonArray types = QJsonArray();
            for (int j = 0; j < header->rowCount(); j++) {
                ProtocalDataItem *data = static_cast<ProtocalDataItem *>(header->child(j));
                QJsonObject type = QJsonObject();
                type["name"] = header->child(j, 1)->text();
                type["type"] = data->getType();
                types.append(type);
            }
            frameObject["types"] = types;
            object[QString(i)] = frameObject;
        }
        object["endianess"] = endianess;
        QJsonDocument saveDoc(object);
        file.write(saveDoc.toJson());
        file.close();
    }
}

void TabAdvanced::onRadioLittleBigClicked()
{
    if (radioLittle->isChecked())
        endianess = LITTLE;
    else
        endianess = BIG;
}

void TabAdvanced::updateDecodeParameters()
{
    listProtocals->clear();
    for (int i = 0; i < model->rowCount(); i++) {
        ProtocalHeaderItem *header = static_cast<ProtocalHeaderItem *>(model->item(i));
        Protocal *protocal = new Protocal(header->getHeader());
        for (int j = 0; j < header->rowCount(); j++) {
            ProtocalDataItem *data = static_cast<ProtocalDataItem *>(header->child(j));
            protocal->append(new VarType(data->getType()));
        }
        listProtocals->append(protocal);
    }
}

const QList<Protocal *> &TabAdvanced::getListProtocals()
{
    return *listProtocals;
}

const ENDIANESS &TabAdvanced::getEndianess()
{
    return endianess;
}
