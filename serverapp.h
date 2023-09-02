#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "app.h"
#include <QMediaRecorder>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include "serverwindow.h"


class ServerApp : App
{
public:
	ServerApp(bool const textModeFlag);
	void start();
	void stop();
private:
	QMediaCaptureSession session;
	QAudioInput input;
	QMediaRecorder rec;
	std::unique_ptr<ServerWindow> pServerWindow;
};

#endif // SERVERAPP_H
