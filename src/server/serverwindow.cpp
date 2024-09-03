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
    this->ui->setupUi(this);
    this->connect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
    this->connect(this->ui->audioDeviceComboBox, &QComboBox::activated, this, [this](int const idx)
    {
        ui->startStopBtn->setText("Stop");
        emit srcDeviceChanged(this->ui->audioDeviceComboBox->itemData(idx).value<QAudioDevice>());
    });

    clientsNoLabel.setText("Connected clients: 0");
    volumeLevelBar.setTextVisible(false);
    volumeLevelBar.setRange(0, 100);

    ui->portNumberInput->setValidator(new QIntValidator(1, UINT16_MAX, this));
    ui->portNumberInput->setText(QString::number(CommunicationModule::SERVER_PORT));
    statusBar()->addPermanentWidget(&statusLabel);

    statusBar()->addPermanentWidget(&clientsNoLabel);

    statusBar()->addPermanentWidget(&volumeLevelBar, 2);

    statusLabel.setText("Stopped");
    this->setFixedSize(QSize(413, 555));
    setWindowTitle(tr("RemoteBoomBox Client"));
}

ServerWindow::~ServerWindow()
{
    this->disconnect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
    delete this->ui;
}

void ServerWindow::handleButton()
{
    qDebug() << "Button released";

    if ("Start" == ui->startStopBtn->text())
    {
        try
        {
            this->serverApp.start(getCurrentAudioDevice(), ui->netIfcComboBox->currentData().value<QHostAddress>(), ui->portNumberInput->text().toUInt());
            ui->startStopBtn->setText("Stop");
            statusLabel.setText("Running");
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
        this->serverApp.stop();
        ui->startStopBtn->setText("Start");
        statusLabel.setText("Stopped");
    }
}

void ServerWindow::clientConnected(uint32_t client_no, Client const &newClient)
{
    clientsNoLabel.setText(QString("Connected clients: %1").arg(client_no));
}

void ServerWindow::fillDevicesSelector(QList<QAudioDevice> const &devices)
{
    for (auto &dev : devices) {
        this->ui->audioDeviceComboBox->addItem(dev.description(), QVariant::fromValue(dev));
    }

    this->ui->audioDeviceComboBox->setCurrentIndex(0);
}

void ServerWindow::fillNetAddresses(QList<QHostAddress> const &ips)
{
    for (auto &ip : ips)
    {
        this->ui->netIfcComboBox->addItem(ip.toString(), QVariant::fromValue(ip));
    }

    this->ui->netIfcComboBox->setCurrentIndex(0);
}

void ServerWindow::updateVolumeLevel(double volumeLevel)
{
    auto static avg = 0.0;

    avg = std::max(volumeLevel, ((avg * 9u) + volumeLevel) / 10u);

    volumeLevelBar.setValue(std::min((int)avg, volumeLevelBar.maximum()));
}

QAudioDevice ServerWindow::getCurrentAudioDevice()
{
    return this->ui->audioDeviceComboBox->currentData().value<QAudioDevice>();
}
