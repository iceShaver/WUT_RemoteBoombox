#include "visualizationfreqspectrumplot.h"
#include "server/soundcollector.h"

QWidget *VisualizationFreqSpectrumPlot::getWidget(void)
{
    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;

    mpCustomPlot = new QCustomPlot();
    mpCustomPlot->legend->setVisible(false);
    mpCustomPlot->yAxis->setLabelColor(Qt::white);
    mpCustomPlot->yAxis->setLabel("SPL [dB]");
    mpCustomPlot->xAxis->setLabelColor(Qt::white);
    mpCustomPlot->xAxis->setLabel("Frequency [Hz]");
    mpCustomPlot->xAxis->setRange(RANGE_START, RANGE_END);
    mpCustomPlot->yAxis->setRange(0.0, 10.0);
    mpCustomPlot->clearGraphs();
    mpCustomPlot->addGraph();
    mpCustomPlot->graph()->setPen(QPen(Qt::red, 2));
    mpCustomPlot->graph()->setName("Sound frequency spectrum");
    mpCustomPlot->setBackground(Qt::transparent);
    mpCustomPlot->xAxis->grid()->setSubGridVisible(false);
    mpCustomPlot->yAxis->grid()->setSubGridVisible(false);

    for (auto i = 0u; i < USEFUL_SPECTOGRAM_DATA_LEN; ++i)
    {
        x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    return mpCustomPlot;
}


void VisualizationFreqSpectrumPlot::update(QVector<double> &spectogram)
{
    mpCustomPlot->graph()->setData(x_axis_vals, spectogram);
    mpCustomPlot->replot();
}
