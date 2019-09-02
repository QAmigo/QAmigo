#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QWidget>

#include "SimpleTab/serialsendbox.h"
#include "SimpleTab/tabcomsimple.h"
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

public slots:
    void errorMessage(QString str);
    void openSerial();

private slots:
    void onButtonRefreshClicked();

private:
    Ui::MainWindow *ui;
    SerialSendBox *sendBox;
    QSerialPort *port;
    TabCOMSimple *tabCOMSimple;
    TabAdvanced *tabAdvanced;
    Decoder *decoder;
};

#endif // MAINWINDOW_H
