#include "clientapp.h"
#include "visualizationfreqspectrumplot.h"
#include "visualizationvolumebar.h"
#include "visualizationcolor.h"
#include "clientwindow.h"

ClientApp::ClientApp() : mClientWindow(*this)
{

    mClientWindow.show();

    connect(&commModule, &CommunicationModule::serverConnected,
            &mClientWindow, &ClientWindow::connected);

    connect(&commModule, &CommunicationModule::serverDisconnected,
            &mClientWindow, &ClientWindow::disconnected);
}

ClientApp::~ClientApp() {}

void ClientApp::setVisualization(EVisualization const vis)
{

    mClientWindow.setVisualizationWidget(nullptr);
    pActiveVisualization.release();

    switch (vis) {
        case EVisualization::FREQ_SPECTRUM_PLOT:
            pActiveVisualization = std::make_unique<VisualizationFreqSpectrumPlot>();
            break;

        case EVisualization::FREQ_SPECTRUM_BARS:
            pActiveVisualization = std::make_unique<VisualizationVolumeBar>();
            break;

        case EVisualization::COLOR:
            pActiveVisualization = std::make_unique<VisualizationColor>();
            break;

        default:
            qDebug() << "Unknown visualization";
            pActiveVisualization = nullptr;
            break;
    }

    if (nullptr != pActiveVisualization)
    {
        mClientWindow.setVisualizationWidget(pActiveVisualization->getWidget());
    }
}

void ClientApp::start(QHostAddress serverAddr, uint16_t serverPort)
{
    commModule.initClientConnection(serverAddr, serverPort);

    QObject::connect(&commModule, &CommunicationModule::audioDataRcvd, this,
        [&](auto const &msg)
        {
            QVector<double> spectogram;
            spectogram.reserve(USEFUL_SPECTOGRAM_DATA_LEN);
            std::copy(msg.data, msg.data + USEFUL_SPECTOGRAM_DATA_LEN, std::back_inserter(spectogram));

            if (nullptr != pActiveVisualization)
            {
                pActiveVisualization->update(spectogram);
            }
        }
    );
}

void ClientApp::stop()
{
    commModule.stop();
    commModule.disconnect(this);

    pActiveVisualization = nullptr;
}
