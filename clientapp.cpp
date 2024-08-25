#include "clientapp.h"

ClientApp::ClientApp(bool const textModeFlag)
    : App(textModeFlag)
{
    if (false == textModeFlag) {
        pClientWindow = std::make_unique<ClientWindow>(*this);
        pClientWindow->show();
    }

    pCommModule->beginRcv();
    QObject::connect(pCommModule.get(), &CommunicationModule::rcvd, [&](CommunicationModule::AppDataMsg const msg) {
//        qDebug() << "Received: ";
        qDebug() << "Rcvd id=0x" << Qt::hex << msg.msgId << " seq_no=0x" << msg.seqNo << " val=" << *(double *)&msg.data;
        this->pClientWindow->setColor(1000000 * *(double *)&msg.data);
    });
//	pUdpSocket->bind(QHostAddress::Any, 53123);

//	qDebug() << "Listener started";
//	QObject::connect(pUdpSocket.get(), &QUdpSocket::readyRead, [this]() {
//		auto buf = std::array<char, 255>();
//		pUdpSocket->readDatagram(buf.data(), buf.size());
//		qDebug() << "read:";
//		for (auto const &byte : buf) {
//			qDebug() << byte;
//		}
//	});
}
