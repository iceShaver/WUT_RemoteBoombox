#ifndef VISUALIZATIONVOLUMEBAR_H
#define VISUALIZATIONVOLUMEBAR_H

#include <QBarSet>
#include "visualization.h"

/**
 * @brief Frequency spectrum bars visualization
 */
class VisualizationVolumeBar : public Visualization
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
    QVector<double> x_axis_vals;
    QVector<QBarSet *> pSets;

};

#endif // VISUALIZATIONVOLUMEBAR_H
