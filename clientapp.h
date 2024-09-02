#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "clientwindow.h"
#include "app.h"
#include "visualization.h"

class ClientApp : public App
{
public:
    ClientApp();

    void setVisualization(EVisualization const vis);

private:
    std::shared_ptr<ClientWindow> pClientWindow = nullptr;
    std::unique_ptr<Visualization> pActiveVisualization = nullptr;

};

#endif // CLIENTAPP_H
