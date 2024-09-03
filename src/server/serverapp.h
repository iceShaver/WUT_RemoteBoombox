#ifndef SERVERAPP_H
#define SERVERAPP_H

#include <QMediaRecorder>
#include <QAudioInput>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QAudioFormat>
#include <QAudioSource>
#include "serverwindow.h"
#include "common/app.h"
#include "soundcollector.h"


/**
 * @brief The ServerApp class
 */
class ServerApp : public App
{
    friend ServerWindow;
public:
    /**
     * @brief ServerApp constructor
     * @param textModeFlag Flag indicating whether application should be started without window
     */
    ServerApp();
    ~ServerApp();

    /**
     * @brief Function begins sound collecting and sending
     */
    void start(QAudioDevice dev, QHostAddress addr, uint16_t port);

    /**
     * @brief stop Function stops sound collecting and sending
     */
    void stop(void);

public slots:
    void newData(double *const p_data, uint32_t const len);
    void srcDeviceChanged(QAudioDevice const &dev);



private:

    ServerWindow serverWindow; /**< Application window object */
    std::unique_ptr<SoundCollector> pSc;
    QVector<double> x_axis_vals;

};

#endif // SERVERAPP_H
