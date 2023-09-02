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
}

void ServerApp::start()
{
	qDebug() << "star rec";
	for (auto const &dev : QMediaDevices::audioInputs()) {
		qDebug() << "ID:" << dev.id() << " desc: " << dev.description();
		if (dev.description().contains("Stereo Mix")) {
			input.setDevice(dev);
			qDebug() << "selected ID:" << dev.id() << " desc: " << dev.description();
		}
	}
	session.setAudioInput(&input);
	session.setRecorder(&rec);
	rec.setQuality(QMediaRecorder::HighQuality);
	rec.setMediaFormat(QMediaFormat::FileFormat::MP3);
	rec.setOutputLocation(QUrl::fromLocalFile("C:\\Users\\kamil\\test"));
	rec.record();
	qDebug()<<rec.error();
}

void ServerApp::stop()
{
	qDebug() << "stop rec";
	rec.stop();
	qDebug()<<rec.error();
}
