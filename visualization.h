#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "clientwindow.h"

enum class EVisualization {
    SPECTOGRAM,
    VOLUME_BAR,
    COLOR
};

class Visualization
{
public:
    Visualization(std::shared_ptr<ClientWindow> pClientWindow);

    virtual QWidget *getWidget(void) = 0;
    virtual void update(QVector<double> &spectogram) = 0;

protected:
    std::shared_ptr<ClientWindow> pClientWindow = nullptr;
};

#endif // VISUALIZATION_H
