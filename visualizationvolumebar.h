#ifndef VISUALIZATIONVOLUMEBAR_H
#define VISUALIZATIONVOLUMEBAR_H

#include "visualization.h"
#include "QBarSet"

class VisualizationVolumeBar : public Visualization
{
    using Visualization::Visualization;
public:

    QWidget *getWidget(void) override;
    void update(QVector<double> &spectogram) override;
    QWidget *pWidget;

private:
    QVector<double> x_axis_vals;
    QVector<QBarSet *> pSets;

};

#endif // VISUALIZATIONVOLUMEBAR_H
