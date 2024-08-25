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


ServerApp::ServerApp(bool const textModeFlag)
    : App(textModeFlag)
{
    if (false == textModeFlag) {
        pServerWindow = std::make_unique<ServerWindow>(*this);
        pServerWindow->show();
    }

    qDebug() << "ServerApp";
}

void ServerApp::start()
{
    qDebug() << "RESUME";
}

void ServerApp::stop()
{
    qDebug() << "SUSPEND";
}
