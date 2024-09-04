#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "clientwindow.h"

/**
 * @brief Enumeration of available Visualizations
 */
enum class EVisualization {
    FREQ_SPECTRUM_PLOT,
    FREQ_SPECTRUM_BARS,
    COLOR
};

/**
 * @brief The abstract Visualization class
 */
class Visualization
{
public:
    /**
     * @brief Visualization constructor
     */
    Visualization();

    /**
     * @brief Visualization desctructor
     */
    virtual ~Visualization();

    /**
     * @brief Virtual function which returns visualization widget
     * @return Pointer to the visualization widget
     */
    virtual QWidget *getWidget(void) = 0;

    /**
     * @brief Virtual function which updates visualization with audio data
     * @param audioData Reference to received audio data
     */
    virtual void update(QVector<double> &audioData) = 0;
};

#endif // VISUALIZATION_H
