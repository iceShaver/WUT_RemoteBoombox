#include "clientapp.h"

ClientApp::ClientApp(bool const textModeFlag)
	: App(textModeFlag)
{
	pUdpSocket->bind(QHostAddress::Any, 53123);

	qDebug() << "Listener started";
	QObject::connect(pUdpSocket.get(), &QUdpSocket::readyRead, [this]() {
		auto buf = std::array<char, 255>();
		pUdpSocket->readDatagram(buf.data(), buf.size());
		qDebug() << "read:";
		for (auto const &byte : buf) {
			qDebug() << byte;
		}
	});
}
