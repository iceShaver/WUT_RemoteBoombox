#ifndef VISUALIZATIONCOLOR_H
#define VISUALIZATIONCOLOR_H

#include "visualization.h"

/**
 * @brief Color background visualization
 */
class VisualizationColor : public Visualization
{

public:
    /**
     * @brief Function returns visualization widget
     * @return Pointer to the visualization widget
     */
    QWidget *getWidget(void) override;

    /**
     * @brief Function updates visualization with audio data
     * @param audioData Reference to received audio data
     */
    void update(QVector<double> &spectogram) override;

private:
    void setNextColor(void);
    QWidget *mpWidget;
    QPalette palette;
    Qt::GlobalColor currentColor;
};

#endif // VISUALIZATIONCOLOR_H
