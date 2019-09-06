#ifndef DATAVISUALIZATIONGRAPH_H
#define DATAVISUALIZATIONGRAPH_H

#include <QBasicTimer>
#include <QtCharts>
#include <QGraphicsView>
#include <QPushButton>
#include <QWidget>

class DataVisualizationGraph : public QWidget
{
    Q_OBJECT
public:
    explicit DataVisualizationGraph(QWidget *parent = nullptr);
    ~DataVisualizationGraph();

signals:
    void onGraphUpdate(int currentX);

public:
    void appendData(const QList<double> &data);
    QLineSeries *createSeries(QString name);
    void removeSeries(QLineSeries *series);
    void updateAxis(double value);

    void setAllowRunning(bool value);

public slots:

private slots:
    void onButtonClearClicked();
    void retranslateUi();

private:
    QGraphicsView *graph;
    QPushButton *buttonClearGraph;
    QChart *chart;
    QBasicTimer *timerUpdate;
    int currentX;
    double max;
    double min;
    int chartWidth;

    bool allowRunning;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) override;

    // QWidget interface
protected:
    void changeEvent(QEvent *event) override;
};

#endif // DATAVISUALIZATIONGRAPH_H
