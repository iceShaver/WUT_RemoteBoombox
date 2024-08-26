#include "visualizationspectogram.h"
#include "soundcollector.h"

QWidget *VisualizationSpectogram::getWidget(void)
{
    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;

    this->pCustomPlot = new QCustomPlot(this->pClientWindow.get());
    this->pCustomPlot->legend->setVisible(false);
    this->pCustomPlot->yAxis->setLabelColor(Qt::white);
    this->pCustomPlot->yAxis->setLabel("SPL [dB]");
    this->pCustomPlot->xAxis->setLabelColor(Qt::white);
    this->pCustomPlot->xAxis->setLabel("Frequency [Hz]");
    this->pCustomPlot->xAxis->setRange(RANGE_START, RANGE_END);
    this->pCustomPlot->yAxis->setRange(0.0, 10.0);
    this->pCustomPlot->clearGraphs();
    this->pCustomPlot->addGraph();
    this->pCustomPlot->graph()->setPen(QPen(Qt::red, 2));
    this->pCustomPlot->graph()->setName("Spectrum");
    this->pCustomPlot->setBackground(Qt::transparent);
    this->pCustomPlot->xAxis->grid()->setSubGridVisible(false);
    this->pCustomPlot->yAxis->grid()->setSubGridVisible(false);

    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i) {
        this->x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    return this->pCustomPlot;
}


void VisualizationSpectogram::update(QVector<double> &spectogram)
{
    this->pCustomPlot->graph()->setData(this->x_axis_vals, spectogram);
    this->pCustomPlot->replot();
}
