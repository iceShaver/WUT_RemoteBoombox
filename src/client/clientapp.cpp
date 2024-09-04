#include "clientapp.h"
#include "visualizationspectogram.h"
#include "visualizationvolumebar.h"
#include "visualizationcolor.h"
#include "clientwindow.h"
#include "common/config.h"

ClientApp::ClientApp()
{
    if (false == Config::textMode) {
        pClientWindow = std::make_shared<ClientWindow>(*this);
        pClientWindow->show();
    }

    // start(QHostAddress(CommunicationModule::SERVER_ADDR), CommunicationModule::SERVER_PORT);
    this->connect(&commModule, &CommunicationModule::serverConnected, pClientWindow.get(), &ClientWindow::connected);
    this->connect(&commModule, &CommunicationModule::serverDisconnected, pClientWindow.get(), &ClientWindow::disconnected);
}

ClientApp::~ClientApp()
{
}

void ClientApp::setVisualization(EVisualization const vis)
{
    switch (vis) {
    case EVisualization::SPECTOGRAM:
        this->pActiveVisualization = std::make_unique<VisualizationSpectogram>(pClientWindow);
        break;

    case EVisualization::VOLUME_BAR:
        this->pActiveVisualization = std::make_unique<VisualizationVolumeBar>(pClientWindow);
        break;

    case EVisualization::COLOR:
        this->pActiveVisualization = std::make_unique<VisualizationColor>(pClientWindow);
        break;

    default:
        qDebug() << "Unknown visualization";
        this->pActiveVisualization = nullptr;
        break;
    }

    if (nullptr != this->pActiveVisualization)
    {
        QWidget *pVisualizationWidget = this->pActiveVisualization->getWidget();
        this->pClientWindow->setVisualizationWidget(pVisualizationWidget);
    }
}

void ClientApp::start(QHostAddress serverAddr, uint16_t serverPort)
{
    commModule.initClientConnection(serverAddr, serverPort);
    setVisualization(EVisualization::VOLUME_BAR);

    QObject::connect(&commModule, &CommunicationModule::rcvd, &commModule, [&](CommunicationModule::AppDataMsg const msg)
    {
        QVector<double> spectogram;
        spectogram.reserve(USEFUL_SPECTOGRAM_DATA_LEN);
        std::copy(msg.data, msg.data + USEFUL_SPECTOGRAM_DATA_LEN, std::back_inserter(spectogram));
        // this->pClientWindow->updateSpectogram(spectogram);

        if (nullptr != this->pActiveVisualization)
        {
            this->pActiveVisualization->update(spectogram);
        }
    });
}

void ClientApp::stop()
{
    commModule.stop();
    commModule.disconnect();
    this->pActiveVisualization = nullptr;
}
