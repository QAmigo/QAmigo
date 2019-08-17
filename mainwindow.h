#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#include "serialsendbox.h"

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
};

#endif // MAINWINDOW_H
