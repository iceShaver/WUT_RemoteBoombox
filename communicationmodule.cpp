#include <QDebug>
#include <QNetworkDatagram>
#include "communicationmodule.h"


CommunicationModule::CommunicationModule()
    : pUdpSocket(new QUdpSocket()),
    txSeqNo(0)
{
    pUdpSocket->bind(QHostAddress::LocalHost, PORT);
    connect(this->pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::rcv);
}

void CommunicationModule::snd(const AudioDataType &audioData)
{
    qDebug() << "sending new data:" << audioData;
    auto const msg = AppDataMsg{.msgId = DATA_MSG_ID, .seqNo = txSeqNo, .data = *(uint64_t*)&audioData};
    pUdpSocket->writeDatagram((char const *) &msg, sizeof(msg), QHostAddress(ADDR), PORT);
    ++txSeqNo;
}

void CommunicationModule::beginRcv()
{
    qDebug() << __FUNCSIG__;
//	QObject::connect(pUdpSocket.get(), &QUdpSocket::readyRead, [this]() {
//		auto buf = std::array<char, 255>();
//		pUdpSocket->readDatagram(buf.data(), buf.size());
//		qDebug() << "[CommMod] rcvd data";
//		auto const msg = *(AppDataMsg *) buf.data();
//		emit rcvd(msg);
//	});

}

void CommunicationModule::rcv()
{
    while (pUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram d = pUdpSocket->receiveDatagram();
        QByteArray arr = d.data();
        AppDataMsg const *p_msg = (AppDataMsg const *)arr.constData();
        qDebug() << "rcvd id=0x" << Qt::hex <<p_msg->msgId << " seq_no=0x" << p_msg->seqNo << " val=" << *(double *)&p_msg->data;
        emit rcvd(*p_msg);
    }
}
