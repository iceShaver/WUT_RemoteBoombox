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

public slots:
    void handleButton(void);

private:
    void newData(double *const p_data, uint32_t const len);
    Ui::ServerWindow *ui;
    ServerApp &serverApp;
    QVector<double> x_axis_vals;
    SoundCollector sc;
};

#endif // SERVERWINDOW_H
