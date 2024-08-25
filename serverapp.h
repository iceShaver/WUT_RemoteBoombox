#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "app.h"
#include "soundcollector.h"
#include <QMediaRecorder>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QAudioFormat>
#include <QAudioSource>
#include "serverwindow.h"
#include "QMediaDevices"

class ServerApp : public App
{
public:
    ServerApp(bool const textModeFlag);
    void start();
    void stop();
    void toggleMode();

private:
    std::unique_ptr<ServerWindow> pServerWindow;
    std::unique_ptr<SoundCollector> pSoundCollector;
};

#endif // SERVERAPP_H
