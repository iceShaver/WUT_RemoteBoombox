#ifndef VISUALIZATIONSPECTOGRAM_H
#define VISUALIZATIONSPECTOGRAM_H

#include "visualization.h"
#include "qcustomplot.h"

/**
 * @brief Frequency spectrum plot visualization
 */
class VisualizationFreqSpectrumPlot : public Visualization
{
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
    QCustomPlot *mpCustomPlot;
    QVector<double> x_axis_vals;

};

#endif // VISUALIZATIONSPECTOGRAM_H
