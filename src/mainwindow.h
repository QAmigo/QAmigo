#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "Connections/commdevice.h"
#include "Connections/serialdevice.h"
#include "Connections/netsocketdevice.h"
#include "SimpleTab/sendbox.h"
#include "SimpleTab/tabcomsimple.h"
#include "tabplugininterface.h"
#include "AdvancedTab/tabadvanced.h"
#include "AdvancedTab/decoder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void updatePluginConnection();
    void translateTo(QString locale);
    void retranslateUi();

public slots:
    void log(QString str);

private slots:
    void onLoadPluginTriggered();
    void onDecodedDataReady(int id, QList<double> listValues);
    void onActionEnglishTriggered();
    void onActionChineseTriggered();

    void onConnectionTypeChanged(bool isChecked);
    void openConnection();

    void onDeviceConnected();
    void onDeviceErrorDisconnected();

private:
    void closeDevice();

private:
    Ui::MainWindow *ui;

    CommDevice *commDevice;
    SerialDevice *serialDevice;
    NetSocketDevice *netSocketDevice;
    QIODevice *currentConnection;

    TabCOMSimple *tabCOMSimple;
    TabAdvanced *tabAdvanced;
    Decoder *decoder;

    QList<TabPluginInterface *> listPlugins;
    QTranslator *translator;

    // QWidget interface
protected:
    void changeEvent(QEvent *event) override;
};

#endif // MAINWINDOW_H
