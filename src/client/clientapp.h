#ifndef CLIENTAPP_H
#define CLIENTAPP_H
#include "common/app.h"
#include "clientwindow.h"
#include "visualization.h"

class ClientApp : public App
{
public:
    ClientApp();
    ~ClientApp();

    void setVisualization(EVisualization const vis);
    void start(QHostAddress serverAddr, uint16_t serverPort);
    void stop();


private:
    std::shared_ptr<ClientWindow> pClientWindow = nullptr;
    std::unique_ptr<Visualization> pActiveVisualization = nullptr;

};

#endif // CLIENTAPP_H
