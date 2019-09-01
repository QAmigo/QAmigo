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
            chart->axes(Qt::Vertical).first()->setMax(data[i] * 1.2);
        }
        if (data[i] < min) {
            min = data[i];
            chart->axes(Qt::Vertical).first()->setMin(data[i] * 1.2);
        }

        (*listSeries)[i]->append(currentX, data[i]);
    }

    currentX++;
    if (currentX > 100) {
        chart->axes(Qt::Horizontal).first()->setRange(currentX - 100, currentX);
        for (auto series : *listSeries)
            if (series->count() > 100)
                series->remove(0);
    }
}

QLineSeries *DataVisualizationGraph::createSeries(QString name)
{
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
    listSeries->append(series);
    series->setName(name);
    return series;
}

void DataVisualizationGraph::removeSeries(QLineSeries *series)
{
    chart->removeSeries(series);
    chart->createDefaultAxes();
    listSeries->removeOne(series);
    if (chart->series().count() == 0) {
        max = 0;
        min = 0;
    }
}
