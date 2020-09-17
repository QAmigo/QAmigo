#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef HOT_PLUG
    #include "qdevicewatcher.h"
#endif

#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    port(new QSerialPort()),
    listPlugins(QList<TabPluginInterface *>()),
    translator(new QTranslator())
{
    ui->setupUi(this);

    refreshPorts();

    ui->comboBaudrate->addItem("1200");
    ui->comboBaudrate->addItem("2400");
    ui->comboBaudrate->addItem("4800");
    ui->comboBaudrate->addItem("9600");
    ui->comboBaudrate->addItem("115200");
    ui->comboBaudrate->setCurrentIndex(4);

    ui->comboDataBits->addItem("5", 5);
    ui->comboDataBits->addItem("6", 6);
    ui->comboDataBits->addItem("7", 7);
    ui->comboDataBits->addItem("8", 8);
    ui->comboDataBits->setCurrentIndex(3);

    ui->comboParity->addItem(tr("NoParity"),	0);
    ui->comboParity->addItem(tr("EvenParity"),	2);
    ui->comboParity->addItem(tr("OldParity"),	3);
    ui->comboParity->addItem(tr("SpaceParity"),	4);
    ui->comboParity->addItem(tr("MarkParity"),	5);
    ui->comboParity->setCurrentIndex(0);

    ui->comboStopBits->addItem("1",   1);
    ui->comboStopBits->addItem("2",   2);
    ui->comboStopBits->addItem("1.5", 3);
    ui->comboStopBits->setCurrentIndex(0);

    ui->comboFlowControl->addItem(tr("No"), 0);
    ui->comboFlowControl->addItem(tr("Hard"), 1);
    ui->comboFlowControl->addItem(tr("Soft"), 2);
    ui->comboFlowControl->setCurrentIndex(0);

    tabCOMSimple = new TabCOMSimple(this, port);
    ui->tabMain->addTab(tabCOMSimple, tr("Simple"));

    tabAdvanced = new TabAdvanced(this);
    ui->tabMain->addTab(tabAdvanced, tr("Advanced"));

    decoder = new Decoder(this, tabAdvanced->getListProtocals(), tabAdvanced->getEndianess());

    connect(ui->buttonOpen, &QPushButton::clicked, this, &MainWindow::openSerial);

    connect(tabCOMSimple, &TabCOMSimple::errorMessage, this, &MainWindow::errorMessage);
    connect(decoder, &Decoder::rawDataReady, tabCOMSimple, &TabCOMSimple::rawDataReady);
    connect(decoder, &Decoder::frameReady, tabAdvanced, &TabAdvanced::frameDataReady);
    connect(ui->buttonRefreshPorts, &QPushButton::clicked, this, &MainWindow::onButtonRefreshClicked);
    connect(ui->actionLoad_Plugin, &QAction::triggered, this, &MainWindow::onLoadPluginTriggered);
    connect(decoder, &Decoder::frameReady, this, &MainWindow::onDecodedDataReady);

    QString folderString = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/QSerial Socket Amigo";
    QFileInfo folder(folderString);
    if (!folder.exists())
        QDir().mkdir(folderString);
    folderString.append("/plugins");
    QFileInfo pluginsFolder(folderString);
    if (!pluginsFolder.exists())
        QDir().mkdir(folderString);

    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onActionEnglishTriggered);
    ui->menuLanguage->addAction(tr("简体中文"), this, &MainWindow::onActionChineseTriggered);

#ifdef HOT_PLUG
    QDeviceWatcher *watcher = new QDeviceWatcher;
    connect(watcher, SIGNAL(portChanged()), this, SLOT(deviceChanged()), Qt::ConnectionType::DirectConnection);
    watcher->start();
#endif
}

#ifdef HOT_PLUG
void MainWindow::deviceChanged() {
    refreshPorts();
}
#endif

MainWindow::~MainWindow()
{
    if (port->isOpen())
        port->close();
    delete port;
    delete ui;
}

void MainWindow::refreshPorts()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

#ifdef HOT_PLUG
    // If the port currently in use is unplugged, we should close the connection.
    bool found = false;
    if (port->isOpen()) {
        for (QSerialPortInfo info : infos) {
            if (info.portName().compare(reinterpret_cast<QSerialPort *>(currentConnection)->portName()) == 0)
                found = true;
        }
        if (!found)
            openSerial();
    }
#endif
    ui->comboPorts->clear();
    for (QSerialPortInfo info : infos) {
        ui->comboPorts->addItem(info.portName());
    }
}

void MainWindow::errorMessage(QString str)
{
    QMessageBox::warning(this, tr("Error"), str);
}

void MainWindow::openSerial()
{
    if (ui->comboPorts->currentText() != "") {
        if (port->isOpen()) {
            port->close();
            ui->buttonOpen->setText(tr("Open"));
            tabAdvanced->setAllowRunning(false);
        } else {
            port->setPortName(ui->comboPorts->currentText());
            bool ok;
            qint32 baudrate = ui->comboBaudrate->currentText().toInt(&ok);
            if (ok)
                port->setBaudRate(baudrate);
            else {
                QMessageBox::warning(this, tr("Error"), tr("Baudrate parse failed"));
                return;
            }

            QSerialPort::DataBits dataBits = QSerialPort::Data8;
            switch(ui->comboDataBits->currentData().toInt()) {
            case 5:
                dataBits = QSerialPort::Data5;
                break;
            case 6:
                dataBits = QSerialPort::Data6;
                break;
            case 7:
            dataBits = QSerialPort::Data7;
            break;
            case 8:
                dataBits = QSerialPort::Data8;
                break;
            default:
                qCritical("BUG: undefined serial databits.");
                break;
            }
            port->setDataBits(dataBits);

            QSerialPort::Parity parity = QSerialPort::NoParity;
            switch (ui->comboParity->currentData().toInt()) {
            case 0:
                parity = QSerialPort::NoParity;
                break;
            case 2:
                parity = QSerialPort::EvenParity;
                break;
            case 3:
                parity = QSerialPort::OddParity;
                break;
            case 4:
                parity = QSerialPort::SpaceParity;
                break;
            case 5:
                parity = QSerialPort::MarkParity;
                break;
            default:
                qCritical("BUG: undefined serial parity.");
                break;
            }
            port->setParity(parity);

            QSerialPort::StopBits stopBits = QSerialPort::OneStop;
            switch (ui->comboStopBits->currentData().toInt()) {
            case 1:
                stopBits = QSerialPort::OneStop;
                break;
            case 2:
                stopBits = QSerialPort::TwoStop;
                break;
            case 3:
                stopBits = QSerialPort::OneAndHalfStop;
                break;
            default:
                qCritical("BUG: undefined serial stopits.");
                break;
            }
            port->setStopBits(stopBits);

            QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
            switch (ui->comboFlowControl->currentData().toInt()) {
            case 0:
                flowControl = QSerialPort::NoFlowControl;
                break;
            case 1:
                flowControl = QSerialPort::HardwareControl;
                break;
            case 2:
                flowControl = QSerialPort::SoftwareControl;
                break;
            default:
                qCritical("BUG: undefined serial Flow Control Type.");
                break;
            }
            port->setFlowControl(flowControl);

            if (!port->open(QIODevice::ReadWrite)) {
                QMessageBox::warning(this, tr("Serial open Failed"), tr("Port open Failed"));
                return;
            }
            ui->buttonOpen->setText(tr("Close"));

            decoder->setConnection(port);
            tabAdvanced->setAllowRunning(true);
            currentConnection = port;
        }
    }
}

void MainWindow::onButtonRefreshClicked()
{
    refreshPorts();
}

void MainWindow::onLoadPluginTriggered()
{
    QString pluginsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/QSerial Socket Amigo/plugins";
#if defined(Q_OS_WIN)
    QString type = tr("Dynamic Linked Library (*.dll)");
#elif defined (Q_OS_LINUX)
    QString type = tr("Shared Library (*.so)");
#else
    //Not supporting Mac.
    Q_ASSERT(false);
#endif
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Plugin to Load"),
        pluginsFolder,
        type);
    if (fileName.isEmpty())
        return;
    else {
        QPluginLoader loader(fileName);
        QObject *pluginObject = loader.instance();
        if (pluginObject) {
            TabPluginInterface *plugin = qobject_cast<TabPluginInterface *>(pluginObject);
            plugin->setConnection(currentConnection);
            listPlugins.append(plugin);
            QWidget *widget = new QWidget();
            widget->setLayout(plugin->getLayout());
            ui->tabMain->addTab(widget, plugin->getName());
            ui->tabMain->setCurrentIndex(ui->tabMain->count() - 1);
        } else
            QMessageBox::warning(this, tr("error"), tr("plugin read error"));
    }
}

void MainWindow::onDecodedDataReady(int id, QList<double> listValues)
{
    for (auto plugin : listPlugins)
        plugin->onFrameUpdated(id, listValues);
}

// Two characters locale, eg: en, zh, de.
void MainWindow::translateTo(QString locale)
{
    QString qmPath = qApp->applicationDirPath().append("/languages/Serial-Amigo_");
    translator->load(qmPath.append(locale).append(".qm"));
    qApp->installTranslator(translator);
}

void MainWindow::retranslateUi()
{
    ui->tabMain->setTabText(1, QCoreApplication::translate("MainWindow", "Simple"));
    ui->tabMain->setTabText(2, QCoreApplication::translate("MainWindow", "Advanced"));
    ui->comboParity->setItemText(0, QCoreApplication::translate("MainWindow", "NoParity"));
    ui->comboParity->setItemText(1, QCoreApplication::translate("MainWindow", "EvenParity"));
    ui->comboParity->setItemText(2, QCoreApplication::translate("MainWindow", "OldParity"));
    ui->comboParity->setItemText(3, QCoreApplication::translate("MainWindow", "SpaceParity"));
    ui->comboParity->setItemText(4, QCoreApplication::translate("MainWindow", "MarkParity"));
    ui->comboFlowControl->setItemText(0, QCoreApplication::translate("MainWindow", "No"));
    ui->comboFlowControl->setItemText(1, QCoreApplication::translate("MainWindow", "Hard"));
    ui->comboFlowControl->setItemText(2, QCoreApplication::translate("MainWindow", "Soft"));
}

void MainWindow::onActionChineseTriggered()
{
    translateTo("zh");
}

void MainWindow::onActionEnglishTriggered()
{
    translateTo("en");
}

void MainWindow::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        retranslateUi();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::updatePluginConnection()
{
    for (auto plugin : listPlugins)
        plugin->setConnection(currentConnection);
}
