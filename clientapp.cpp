#include "clientapp.h"
#include "visualizationspectogram.h"
#include "visualizationvolumebar.h"
#include "visualizationcolor.h"
#include "clientwindow.h"
#include "config.h"

ClientApp::ClientApp()
{
    if (false == Config::textMode) {
        pClientWindow = std::make_shared<ClientWindow>(*this);
        pClientWindow->show();
    }

    commModule.initClientConnection();

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
