#include <QByteArray>
#include <QDebug>
#include <QMediaFormat>
#include <QMediaDevices>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QAudioDevice>
#include <QUrl>
#include <ranges>
#include "serverapp.h"
#include "common/config.h"
#include "common/communicationmodule.h"
#include "soundcollector.h"
#include "fftw3.h"

ServerApp::ServerApp() :
    serverWindow(*this)
{
    if (false == Config::textMode) {
        serverWindow.show();
    }

    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;
    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i) {
        x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    this->serverWindow.fillDevicesSelector(this->pSc->getInputs());

    auto const f = QNetworkInterface::allAddresses()
        | std::views::filter([](auto &ip){return ip.protocol() == QAbstractSocket::IPv4Protocol;})
        | std::ranges::to<QList>();

    this->serverWindow.fillNetAddresses(f);

    this->connect(&commModule, &CommunicationModule::clientConnectionChanged, &serverWindow, &ServerWindow::clientConnected);
    this->connect(&this->serverWindow, &ServerWindow::srcDeviceChanged, this, &ServerApp::srcDeviceChanged);

    qDebug() << "ServerApp";
}

ServerApp::~ServerApp()
{
    QObject::disconnect(this->pSc.get(), &SoundCollector::newData, this, &ServerApp::newData);
}

void ServerApp::start(QAudioDevice dev, QHostAddress addr, uint16_t port)
{
    qDebug() << "START";
    this->commModule.initServerConnection(addr, port);
    this->srcDeviceChanged(dev);
}

void ServerApp::stop(void)
{
    qDebug() << "STOP";

    if (this->pSc)
    {
        this->disconnect(this->pSc.get(), &SoundCollector::newData, this, &ServerApp::newData);
        this->pSc = nullptr;
    }
    this->commModule.stop();
}


void ServerApp::newData(double *const p_data, uint32_t const len)
{
    auto fft_out = std::array<double, FFT_BUF_SIZE>{};
    auto fftVec = QVector<double>{};
    auto p = fftw_plan_r2r_1d(FFT_BUF_SIZE, p_data, fft_out.data(), FFTW_R2HC, FFTW_ESTIMATE);

    fftw_execute(p);
    fftw_destroy_plan(p);

    for (auto i = 0u; i < (len / 4u); ++i) // range 10 HZ - 12 kHz
    {
        fftVec.append(abs(fft_out[i]));
    }

    if (0 != fftVec.size())
    {
        this->serverWindow.updateVolumeLevel(20.0 * (std::accumulate(fftVec.begin(), fftVec.end(), 0.0)) / fftVec.size());
    }

    this->commModule.sndSpectogram(fftVec);

}

void ServerApp::srcDeviceChanged(QAudioDevice const &dev)
{
    if (this->pSc)
    {
        this->disconnect(this->pSc.get(), &SoundCollector::newData, this, &ServerApp::newData);
    }
    this->pSc = std::make_unique<SoundCollector>(dev);
    this->connect(this->pSc.get(), &SoundCollector::newData, this, &ServerApp::newData);
}

