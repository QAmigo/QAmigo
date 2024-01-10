#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listPlugins(QList<TabPluginInterface *>()),
    translator(new QTranslator())
{
    ui->setupUi(this);

    tabCOMSimple = new TabCOMSimple(this);
    ui->tabMain->addTab(tabCOMSimple, tr("Simple"));

    tabAdvanced = new TabAdvanced(this);
    ui->tabMain->addTab(tabAdvanced, tr("Advanced"));

    decoder = new Decoder(this, tabAdvanced->getListProtocals(), tabAdvanced->getEndianess());

    // [start] Data stream connections.
    connect(decoder, &Decoder::rawDataReady, tabCOMSimple, &TabCOMSimple::rawDataReady);
    connect(decoder, &Decoder::frameReady, tabAdvanced, &TabAdvanced::frameDataReady);
    // [End]

    // [Start] Plugins
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
    // [End]

    // [Start] i18n
    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onActionEnglishTriggered);
    ui->menuLanguage->addAction(tr("简体中文"), this, &MainWindow::onActionChineseTriggered);
    // [End]

    ui->groupNetProperties->setDisabled(true);
    connect(ui->radioSerial, &QRadioButton::clicked, this, &MainWindow::onConnectionTypeChanged);
    connect(ui->radioNetSocket, &QRadioButton::clicked, this, &MainWindow::onConnectionTypeChanged);
    connect(ui->buttonOpen, &QPushButton::clicked, this, &MainWindow::openConnection);

    serialDevice = new SerialDevice(this, ui->comboPorts, ui->buttonRefreshPorts,
                                          ui->comboBaudrate, ui->comboDataBits,
                                          ui->comboParity, ui->comboStopBits,
                                          ui->comboFlowControl);
    netSocketDevice = new NetSocketDevice(this, ui->inputNetIPAddr, ui->inputNetPort,
                                          ui->radioNetTypeTCP, ui->radioNetTypeUDP,
                                          ui->radioNetRoleClient, ui->radioNetRoleServer);
    connect(serialDevice, &SerialDevice::connected, this, &MainWindow::onDeviceConnected);
    connect(netSocketDevice, &SerialDevice::connected, this, &MainWindow::onDeviceConnected);
    connect(serialDevice, &SerialDevice::errorDisconnected, this, &MainWindow::onDeviceErrorDisconnected);
    connect(netSocketDevice, &SerialDevice::errorDisconnected, this, &MainWindow::onDeviceErrorDisconnected);
    commDevice = serialDevice;

    // [start] Log stream connections.
    connect(tabCOMSimple, &TabCOMSimple::log, this, &MainWindow::log);
    connect(netSocketDevice, &NetSocketDevice::log, this, &MainWindow::log);
    // [end]

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectionTypeChanged(bool isChecked)
{
    Q_UNUSED(isChecked)

    if (ui->radioSerial->isChecked()) {
        ui->groupNetProperties->setDisabled(true);
        ui->groupSerialProperties->setEnabled(true);
        commDevice = serialDevice;
    } else if (ui->radioNetSocket->isChecked()) {
        ui->groupNetProperties->setEnabled(true);
        ui->groupSerialProperties->setDisabled(true);
        commDevice = netSocketDevice;
    }
}

void MainWindow::closeDevice()
{
    commDevice->close();
    ui->groupConnSel->setEnabled(true);
    ui->buttonOpen->setText(tr("Open Connection"));
    tabAdvanced->setAllowRunning(false);
}

void MainWindow::openConnection()
{
    if (ui->groupConnSel->isEnabled()) {
        // This acts like sending connect command, device will signals conncted if so.
        int ret = commDevice->open();
        if (ret != 0) {
            return;
        }
    } else {
        closeDevice();
    }
}

// Only when device is really connected.
void MainWindow::onDeviceConnected()
{
    tabCOMSimple->bindIODevice(commDevice->ioDevice);
    decoder->setConnection(commDevice->ioDevice);
    ui->groupConnSel->setDisabled(true);
    ui->buttonOpen->setText(tr("Close Connection"));
    tabAdvanced->setAllowRunning(true);
}

void MainWindow::onDeviceErrorDisconnected()
{
    closeDevice();
}

void MainWindow::log(QString str)
{
    ui->textLog->append(str);
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
        plugin->setConnection(commDevice->ioDevice);
}
