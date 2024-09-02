#include "serverapp.h"
#include <QByteArray>
#include <QDebug>
#include <QMediaFormat>
#include <QMediaDevices>
#include <QNetworkDatagram>
#include <QAudioDevice>
#include <QUrl>
#include <string>
#include <utility>
#include "config.h"
#include "communicationmodule.h"

ServerApp::ServerApp() : serverWindow(*this)
{
    if (false == Config::textMode) {
        serverWindow.show();
    }

    this->commModule.initServerConnection();

    this->serverWindow.fillDevicesSelector(this->soundCollector.getInputs());

    QObject::connect(&commModule, &CommunicationModule::clientConnected, &serverWindow, &ServerWindow::clientConnected);

    qDebug() << "ServerApp";
}

void ServerApp::start(void)
{
    qDebug() << "RESUME";
}

void ServerApp::stop(void)
{
    qDebug() << "SUSPEND";
}
