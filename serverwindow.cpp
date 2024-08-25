#include <QMediaFormat>
#include <QAudioDecoder>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QIODevice>
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "serverapp.h"
#include "communicationmodule.h"
#include "fftw3.h"


ServerWindow::ServerWindow(ServerApp &serverApp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow),
    serverApp(serverApp)
{
    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;

    this->ui->setupUi(this);
    this->connect(&this->sc, &SoundCollector::newData, this, &ServerWindow::newData);
    this->connect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);

    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i) {
        x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    ui->widget->legend->setVisible(false);
    ui->widget->yAxis->setLabel("SPL [dB]");
    ui->widget->xAxis->setLabel("Frequency [Hz]");
    ui->widget->xAxis->setRange(RANGE_START, RANGE_END);
    ui->widget->yAxis->setRange(0.0, 10.0);
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph()->setPen(QPen(Qt::red));
    ui->widget->graph()->setName("Spectrum");
}

ServerWindow::~ServerWindow()
{
    delete this->ui;
}

void ServerWindow::handleButton()
{
    qDebug() << "Button released";

    if ("Start" == ui->startStopBtn->text())
    {
        this->sc.start();
        ui->startStopBtn->setText("Stop");
    }
    else
    {
        this->sc.stop();
        ui->startStopBtn->setText("Start");
    }
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

    ui->widget->graph(0)->setData(x_axis_vals, fftVec);
    ui->widget->replot();

//    qDebug() << "Got arr len = " << len << " val = " << p_data[0] << " " << p_data[5] << " " << p_data[20] << " " << p_data[63];
//    qDebug() << "Spectrum size: " << fftVec.size();

    this->serverApp.pCommModule->sndSpectogram(fftVec);
}
