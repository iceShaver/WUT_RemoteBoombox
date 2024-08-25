#include <QDebug>
#include <QNetworkDatagram>
#include "communicationmodule.h"
#include <cstring>


CommunicationModule::CommunicationModule()
    : pUdpSocket(new QUdpSocket()),
    txSeqNo(0)
{

}

void CommunicationModule::snd(const AudioDataType &audioData)
{
//    qDebug() << "sending new data:" << audioData;
//    auto const msg = AppDataMsg{.msgId = DATA_MSG_ID, .seqNo = txSeqNo, .data = *(uint64_t*)&audioData};
//    pUdpSocket->writeDatagram((char const *) &msg, sizeof(msg), QHostAddress(ADDR), PORT);
//    ++txSeqNo;
}

void CommunicationModule::sndSpectogram(QVector<double> &spectogram)
{
//    qDebug() << "sending new spectogram";
    auto msg = AppDataMsg{.msgId = DATA_MSG_ID, .seqNo = txSeqNo};
    memcpy(msg.data, (uint8_t *)spectogram.data(), std::min(spectogram.size(), (qsizetype)USEFUL_SPECTOGRAM_DATA_LEN) * sizeof(double));

    pUdpSocket->writeDatagram((char const *) &msg, sizeof(msg), QHostAddress(ADDR), PORT);
    ++txSeqNo;
}


void CommunicationModule::beginRcv()
{
    qDebug() << __FUNCSIG__;
    pUdpSocket->bind(QHostAddress::LocalHost, PORT);
    connect(this->pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::rcv);

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
//        qDebug() << "rcvd id=0x" << Qt::hex <<p_msg->msgId << " seq_no=0x" << p_msg->seqNo << " val=" << *(double *)&p_msg->data;
        emit rcvd(*p_msg);
    }
}
