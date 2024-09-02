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

/**
 * @brief The ServerApp class
 */
class ServerApp : public App
{
public:
    /**
     * @brief ServerApp constructor
     * @param textModeFlag Flag indicating whether application should be started without window
     */
    ServerApp();

    /**
     * @brief Function begins sound collecting and sending
     */
    void start(void);

    /**
     * @brief stop Function stops sound collecting and sending
     */
    void stop(void);




private:

    ServerWindow serverWindow; /**< Application window object */
    SoundCollector soundCollector; /**< Sound Collector module object */
};

#endif // SERVERAPP_H
