#include "datavisualizationgraph.h"

#include <QtCharts/QChartView>
#include <QVBoxLayout>

DataVisualizationGraph::DataVisualizationGraph(QWidget *parent) : QWidget(parent),
    graph(new QGraphicsView()),
    buttonClearGraph(new QPushButton("Clear")),
    chart(new QChart()),
    listSeries(new QList<QLineSeries *>()),
    chartWidth(100)
{
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

//    chart->addSeries(series);
//    chart->axes(Qt::Horizontal).first()->setRange(0, 100);
//    chart->axes(Qt::Vertical).first()->setRange(0, 1);
    QChartView *qChartView = new QChartView(chart);

    layout->addWidget(qChartView);
    QHBoxLayout *layoutControls = new QHBoxLayout();
    layout->addLayout(layoutControls);
    layoutControls->addWidget(buttonClearGraph);
}

void DataVisualizationGraph::appendData(const QList<double> &data)
{
    for (int i = 0; i < data.count(); i++) {
        if (data[i] > max) {
            max = data[i];
            chart->axes(Qt::Vertical).at(i)->setMax(data[i] * 1.2);
        }
        if (data[i] < min) {
            min = data[i];
            chart->axes(Qt::Vertical).at(i)->setMin(data[i] * 1.2);
        }

        (*listSeries)[i]->append(currentX, data[i]);
    }

    currentX++;
    if (currentX > 100) {
        chart->axes(Qt::Horizontal).first()->setRange(currentX - 100, currentX);
    }
}

void DataVisualizationGraph::createSeries(QString name)
{
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).at(listSeries->count())->setRange(0, 100);
    chart->axes(Qt::Vertical).at(listSeries->count())->setRange(0, 1);
    listSeries->append(series);
    series->setName(name);
}
