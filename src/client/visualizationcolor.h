#ifndef VISUALIZATIONCOLOR_H
#define VISUALIZATIONCOLOR_H

#include "visualization.h"

class VisualizationColor : public Visualization
{
    using Visualization::Visualization;

public:
    QWidget *getWidget(void) override;
    void update(QVector<double> &spectogram) override;

private:
    void setNextColor(void);
    QWidget *pWidget;
    QPalette palette;
    Qt::GlobalColor currentColor;
};

#endif // VISUALIZATIONCOLOR_H
