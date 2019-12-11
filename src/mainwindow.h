#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QWidget>

#include "SimpleTab/serialsendbox.h"
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
    ~MainWindow();

private:
    void refreshPorts();
    void updatePluginConnection();
    void translateTo(QString locale);
    void retranslateUi();

public slots:
    void errorMessage(QString str);
    void openSerial();
    void deviceChanged();

private slots:
    void onButtonRefreshClicked();
    void onLoadPluginTriggered();
    void onDecodedDataReady(int id, QList<double> listValues);
    void onActionEnglishTriggered();
    void onActionChineseTriggered();

private:
    Ui::MainWindow *ui;
    SerialSendBox *sendBox;
    QIODevice *currentConnection;
    QSerialPort *port;
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
