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

/**
 * @brief Server application user interface
 */
class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief ServerWindow constructor
     * @param serverApp Reference to server app
     * @param parent Pointer to the parent
     */
    explicit ServerWindow(ServerApp &serverApp, QWidget *parent = nullptr);

    /**
     * @brief ServerWindow destructor
     */
    ~ServerWindow();

    /**
     * @brief Function fills device selector with available devices
     * @param devices List of available devices
     */
    void fillDevicesSelector(QList<QAudioDevice> const &devices);

    /**
     * @brief Function fills network ip address selector with available adresses
     * @param ips List of available IP adresses
     */
    void fillNetAddresses(QList<QHostAddress> const &ips);

    /**
     * @brief Function updates volume level in the status bar
     * @param volumeLevel calculated current volume level
     */
    void updateVolumeLevel(double volumeLevel);

    /**
     * @brief Funvtion returns currently selected (in UI) audio device
     * @return
     */
    QAudioDevice getCurrentAudioDevice();


public slots:
    /**
     * @brief Button click event slot
     */
    void handleButton(void);

    /**
     * @brief Client connected event slot
     * @param client_no New number of client
     * @param newClient Reference to newly connected client
     */
    void clientConnected(uint32_t client_no, Client const &newClient);

signals:
    /**
     * @brief Audio source device changed event signal
     * @param dev Newwly selected device
     */
    void srcDeviceChanged(QAudioDevice const &dev);


private:
    Ui::ServerWindow *ui;
    ServerApp &serverApp;
    QLabel *mpClientsNoLabel;
    QLabel *mpStatusLabel;
    QProgressBar *mpVolumeLevelBar;
};

#endif // SERVERWINDOW_H
