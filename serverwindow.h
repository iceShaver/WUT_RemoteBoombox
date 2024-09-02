#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QAudioSource>
#include <QMainWindow>
#include <memory>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QMediaDevices>
#include <QAudioInput>
#include "communicationmodule.h"
#include <QUrl>
#include <QProgressBar>
#include <QLabel>
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

public slots:
    void handleButton(void);
    void clientConnected(uint32_t client_no, Client const &newClient);

private:
    void newData(double *const p_data, uint32_t const len);
    void updateVolumeLevel(double volumeLevel);

    Ui::ServerWindow *ui;
    ServerApp &serverApp;
    QVector<double> x_axis_vals;
    std::unique_ptr<SoundCollector> pSc;
    QLabel clientsNoLabel;
    QProgressBar volumeLevelBar;
};

#endif // SERVERWINDOW_H
