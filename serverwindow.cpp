#include <QMediaFormat>
#include <QAudioDecoder>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QIODevice>
#include <QSizePolicy>
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "serverapp.h"
#include "communicationmodule.h"
#include "fftw3.h"


ServerWindow::ServerWindow(ServerApp &serverApp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow),
    serverApp(serverApp),
    pSc(std::make_unique<SoundCollector>())
{
    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;

    this->ui->setupUi(this);
    this->connect(this->pSc.get(), &SoundCollector::newData, this, &ServerWindow::newData);
    this->connect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);

    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i) {
        x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    this->connect(this->ui->audioDeviceComboBox, &QComboBox::activated, this, [this](int const idx) {
        this->disconnect(this->pSc.get(), &SoundCollector::newData, this, &ServerWindow::newData);
        pSc = std::make_unique<SoundCollector>(this->ui->audioDeviceComboBox->itemData(idx).value<QAudioDevice>());
        this->connect(this->pSc.get(), &SoundCollector::newData, this, &ServerWindow::newData);
        ui->startStopBtn->setText("Stop");
    });

    clientsNoLabel.setText("Connected clients: 0");
    volumeLevelBar.setFormat("str");
    volumeLevelBar.setTextVisible(true);
    volumeLevelBar.setRange(0, 100);

    statusBar()->addPermanentWidget(&clientsNoLabel);
    statusBar()->addPermanentWidget(&volumeLevelBar, 2);
}

ServerWindow::~ServerWindow()
{
    this->disconnect(this->pSc.get(), &SoundCollector::newData, this, &ServerWindow::newData);
    this->disconnect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
    delete this->ui;
}

void ServerWindow::handleButton()
{
    qDebug() << "Button released";

    if ("Start" == ui->startStopBtn->text())
    {
        this->pSc->start();
        ui->startStopBtn->setText("Stop");
    }
    else
    {
        this->pSc->stop();
        ui->startStopBtn->setText("Start");
    }
}

void ServerWindow::clientConnected(uint32_t client_no, Client const &newClient)
{
    clientsNoLabel.setText(QString("Connected clients: %1").arg(client_no));
}


void ServerWindow::newData(double *const p_data, uint32_t const len)
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
        this->updateVolumeLevel(20.0 * (std::accumulate(fftVec.begin(), fftVec.end(), 0.0)) / fftVec.size());
    }

    this->serverApp.commModule.sndSpectogram(fftVec);

}

void ServerWindow::fillDevicesSelector(QList<QAudioDevice> const &devices)
{
    for (auto &dev : devices) {
        this->ui->audioDeviceComboBox->addItem(dev.description(), QVariant::fromValue(dev));
    }

    this->ui->audioDeviceComboBox->setCurrentIndex(0);
}

void ServerWindow::updateVolumeLevel(double volumeLevel)
{
    auto static avg = 0.0;

    avg = std::max(volumeLevel, ((avg * 9u) + volumeLevel) / 10u);

    volumeLevelBar.setValue(std::min((int)avg, volumeLevelBar.maximum()));
}
