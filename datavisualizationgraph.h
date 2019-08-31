#ifndef DATAVISUALIZATIONGRAPH_H
#define DATAVISUALIZATIONGRAPH_H

#include <QtCharts>
#include <QGraphicsView>
#include <QPushButton>
#include <QWidget>

class DataVisualizationGraph : public QWidget
{
    Q_OBJECT
public:
    explicit DataVisualizationGraph(QWidget *parent = nullptr);

signals:

public:
    void appendData(const QList<double> &data);
    void createSeries(QString name);

public slots:

private:
    QGraphicsView *graph;
    QPushButton *buttonClearGraph;
    QChart *chart;
    uint32_t currentX;
    double max;
    double min;
    QList<QLineSeries *> *listSeries;
    int chartWidth;
};

#endif // DATAVISUALIZATIONGRAPH_H
