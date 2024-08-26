#ifndef VISUALIZATIONVOLUMEBAR_H
#define VISUALIZATIONVOLUMEBAR_H

#include "visualization.h"

class VisualizationVolumeBar : public Visualization
{
    using Visualization::Visualization;
public:

    QWidget *getWidget(void) override;
    void update(QVector<double> &spectogram) override;
};

#endif // VISUALIZATIONVOLUMEBAR_H
