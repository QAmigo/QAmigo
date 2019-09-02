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
    ~DataVisualizationGraph();

signals:

public:
    void appendData(const QList<double> &data);
    QLineSeries *createSeries(QString name);
    void removeSeries(QLineSeries *series);

public slots:

private slots:
    void onButtonClearClicked();

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
