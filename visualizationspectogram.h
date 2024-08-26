#ifndef VISUALIZATIONSPECTOGRAM_H
#define VISUALIZATIONSPECTOGRAM_H

#include "visualization.h"
#include "qcustomplot.h"

class VisualizationSpectogram : public Visualization
{
    using Visualization::Visualization;

    QWidget *getWidget(void) override;
    void update(QVector<double> &spectogram) override;

private:
    QCustomPlot *pCustomPlot;
    QVector<double> x_axis_vals;

};

#endif // VISUALIZATIONSPECTOGRAM_H
