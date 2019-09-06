#include "datavisualizationgraph.h"

#include <QtCharts/QChartView>
#include <QVBoxLayout>

DataVisualizationGraph::DataVisualizationGraph(QWidget *parent) : QWidget(parent),
    graph(new QGraphicsView()),
    buttonClearGraph(new QPushButton(tr("Clear"))),
    chart(new QChart()),
    timerUpdate(new QBasicTimer()),
    currentX(0),
    max(0),
    min(0),
    chartWidth(100)
{
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    QChartView *qChartView = new QChartView(chart);

    layout->addWidget(qChartView);
    QHBoxLayout *layoutControls = new QHBoxLayout();
    layout->addLayout(layoutControls);
    layoutControls->addWidget(buttonClearGraph);
    connect(buttonClearGraph, &QPushButton::clicked, this, &DataVisualizationGraph::onButtonClearClicked);

}

DataVisualizationGraph::~DataVisualizationGraph()
{
    delete timerUpdate;
}

void DataVisualizationGraph::updateAxis(double value)
{
    if (value > max) {
        max = value;
        chart->axes(Qt::Vertical).first()->setMax(max * 1.2);
    } else if (value < min) {
        min = value;
        chart->axes(Qt::Vertical).first()->setMin(min * 1.2);
    }
}

QLineSeries *DataVisualizationGraph::createSeries(QString name)
{
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
    if (currentX < 100)
        chart->axes(Qt::Horizontal).first()->setRange(0, 100);
    else
        chart->axes(Qt::Horizontal).first()->setRange(currentX - 100, currentX);
    series->setName(name);

    if (!timerUpdate->isActive())
        if (allowRunning)
            timerUpdate->start(20, this);	//Default 50Hz.

    return series;
}

void DataVisualizationGraph::removeSeries(QLineSeries *series)
{
    chart->removeSeries(series);
    chart->createDefaultAxes();
    if (currentX < 100)
        chart->axes(Qt::Horizontal).first()->setRange(0, 100);
    else
        chart->axes(Qt::Horizontal).first()->setRange(currentX - 100, currentX);
    if (chart->series().count() == 0) {
        max = 0;
        min = 0;
    }

    if (chart->series().count() == 0)
        timerUpdate->stop();
}

void DataVisualizationGraph::onButtonClearClicked()
{
}

void DataVisualizationGraph::setAllowRunning(bool value)
{
    allowRunning = value;
    if (!allowRunning)
        timerUpdate->stop();
    else
        if (chart->series().count() != 0)
            timerUpdate->start(20, this);
}

void DataVisualizationGraph::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerUpdate->timerId()) {
        emit onGraphUpdate(currentX++);
        if (currentX > 100)
            chart->axes(Qt::Horizontal).first()->setRange(currentX - 100, currentX);
        for (auto series : chart->series()) {
            QLineSeries *lineSeries = static_cast<QLineSeries *>(series);
            double value = lineSeries->at(lineSeries->count() - 1).y();
             if (value > max) {
                max = value;
                chart->axes(Qt::Vertical).first()->setMax(max * 1.2);
            } else if (value < min) {
                min = value;
                chart->axes(Qt::Vertical).first()->setMin(min * 1.2);
            }
        }
    } else
        QWidget::timerEvent(event);
}

void DataVisualizationGraph::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QWidget::changeEvent(event);
}

void DataVisualizationGraph::retranslateUi()
{
    buttonClearGraph->setText(QCoreApplication::translate("DataVisualizationGraph", "Clear"));
}
