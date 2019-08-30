#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QWidget>

#include "serialsendbox.h"
#include "tabcomsimple.h"
#include "tabadvanced.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void errorMessage(QString str);
    void openSerial();

private:
    Ui::MainWindow *ui;
    SerialSendBox *sendBox;
    QSerialPort *port;
    TabCOMSimple *tabCOMSimple;
    TabAdvanced *tabAdvanced;
};

#endif // MAINWINDOW_H
