#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QAudioSource>
#include <QMainWindow>
#include <memory>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QMediaDevices>
#include <QAudioInput>
#include <QUrl>
#include <QProgressBar>
#include <QLabel>
#include "common/communicationmodule.h"
#include "soundcollector.h"


#define BARS_NO 12u

class ServerApp;

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(ServerApp &serverApp, QWidget *parent = nullptr);
    ~ServerWindow();
    void fillDevicesSelector(QList<QAudioDevice> const &devices);
    void fillNetAddresses(QList<QHostAddress> const &ifcs);
    void updateVolumeLevel(double volumeLevel);
    QAudioDevice getCurrentAudioDevice();


public slots:
    void handleButton(void);
    void clientConnected(uint32_t client_no, Client const &newClient);

signals:
    void srcDeviceChanged(QAudioDevice const &dev);


private:

    Ui::ServerWindow *ui;
    ServerApp &serverApp;
    QLabel clientsNoLabel;
    QLabel statusLabel;
    QProgressBar volumeLevelBar;
};

#endif // SERVERWINDOW_H
