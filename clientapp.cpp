#include "clientapp.h"

ClientApp::ClientApp(bool const textModeFlag)
    : App(textModeFlag)
{
    if (false == textModeFlag) {
        pClientWindow = std::make_unique<ClientWindow>(*this);
        pClientWindow->show();
    }

    pCommModule->beginRcv();
    QObject::connect(pCommModule.get(), &CommunicationModule::rcvd, [&](CommunicationModule::AppDataMsg const msg)
    {
//        qDebug() << "Received: ";

        QVector<double> spectogram;
        spectogram.reserve(USEFUL_SPECTOGRAM_DATA_LEN);
        std::copy(msg.data, msg.data + USEFUL_SPECTOGRAM_DATA_LEN, std::back_inserter(spectogram));
//        qDebug() << "Rcvd id=0x" << Qt::hex << msg.msgId << " seq_no=0x" << msg.seqNo << " val=" << *(double *)&msg.data;
//        this->pClientWindow->setColor(1000000 * *(double *)&msg.data);
        this->pClientWindow->updateSpectogram(spectogram);
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
