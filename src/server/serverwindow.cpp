#include <QMediaFormat>
#include <QAudioDecoder>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QIODevice>
#include <QSizePolicy>
#include <QNetworkInterface>
#include <QMessageBox>
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "serverapp.h"
#include "common/communicationmodule.h"


ServerWindow::ServerWindow(ServerApp &serverApp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow),
    serverApp(serverApp)
{
    ui->setupUi(this);
    connect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
    connect(ui->audioDeviceComboBox, &QComboBox::activated, this, [&](int const idx)
    {
        ui->startStopBtn->setText("Stop");
        emit srcDeviceChanged(ui->audioDeviceComboBox->itemData(idx).value<QAudioDevice>());
    });
    mpClientsNoLabel = new QLabel;
    mpStatusLabel = new QLabel;
    mpVolumeLevelBar = new QProgressBar();
    mpClientsNoLabel->setText("Connected clients: 0");
    mpVolumeLevelBar->setTextVisible(false);
    mpVolumeLevelBar->setRange(0, 100);

    ui->portNumberInput->setValidator(new QIntValidator(1, UINT16_MAX, this));
    ui->portNumberInput->setText(QString::number(CommunicationModule::DEFAULT_SERVER_PORT));
    statusBar()->addPermanentWidget(mpStatusLabel);

    statusBar()->addPermanentWidget(mpClientsNoLabel);

    statusBar()->addPermanentWidget(mpVolumeLevelBar, 2);

    mpStatusLabel->setText("Stopped");
    setFixedSize(QSize(413, 555));
    setWindowTitle(tr("RemoteBoomBox Client"));
}

ServerWindow::~ServerWindow()
{
    disconnect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
    delete ui;
}

void ServerWindow::handleButton()
{
    qDebug() << "Button released";

    if ("Start" == ui->startStopBtn->text())
    {
        try
        {
            serverApp.start(getCurrentAudioDevice(),
                            ui->netIfcComboBox->currentData().value<QHostAddress>(),
                            ui->portNumberInput->text().toUInt()
            );

            ui->startStopBtn->setText("Stop");
            mpStatusLabel->setText("Running");
        }
        catch (std::runtime_error &e)
        {
            QMessageBox msgBox;
            msgBox.critical(this, "Error", e.what());
            msgBox.setFixedSize(300, 200);
        }

    }
    else
    {
        serverApp.stop();
        ui->startStopBtn->setText("Start");
        mpStatusLabel->setText("Stopped");
    }
}

void ServerWindow::clientConnected(uint32_t client_no, Client const &newClient)
{
    mpClientsNoLabel->setText(QString("Connected clients: %1").arg(client_no));
}

void ServerWindow::fillDevicesSelector(QList<QAudioDevice> const &devices)
{
    for (auto &dev : devices) {
        ui->audioDeviceComboBox->addItem(dev.description(), QVariant::fromValue(dev));
    }

    ui->audioDeviceComboBox->setCurrentIndex(0);
}

void ServerWindow::fillNetAddresses(QList<QHostAddress> const &ips)
{
    for (auto &ip : ips)
    {
        ui->netIfcComboBox->addItem(ip.toString(), QVariant::fromValue(ip));
    }

    ui->netIfcComboBox->setCurrentIndex(0);
}

void ServerWindow::updateVolumeLevel(double volumeLevel)
{
    auto static avg = 0.0;

    avg = std::max(volumeLevel, ((avg * 9u) + volumeLevel) / 10u);

    mpVolumeLevelBar->setValue(std::min((int)avg, mpVolumeLevelBar->maximum()));
}

QAudioDevice ServerWindow::getCurrentAudioDevice()
{
    return ui->audioDeviceComboBox->currentData().value<QAudioDevice>();
}
