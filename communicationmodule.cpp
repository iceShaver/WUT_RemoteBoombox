#include <QDebug>
#include <QNetworkDatagram>
#include "communicationmodule.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std::literals;

CommunicationModule::CommunicationModule()
    : pUdpSocket(new QUdpSocket()),
    txSeqNo(0)
{

}

void CommunicationModule::initServerConnection()
{
    pUdpSocket->bind(QHostAddress(SERVER_ADDR), SERVER_PORT);
    connect(this->pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::serverRcv);

    clientManagementThread = std::thread([&]
    {
        while (true)
        {
            clients_mutex.lock();
            for (auto &[k, v] : clients)
            {
                v.delete_flag = true;
            };
            clients_mutex.unlock();

            std::this_thread::sleep_for(1s);

            clients_mutex.lock();
            for (auto it = clients.begin(); it != clients.end();)
            {
                if (it->second.delete_flag)
                {
                    emit clientConnected(clients.size() - 1u, it->second);
                    clients.erase(it++);
                }
                else
                {
                    it++;
                }
            }
            clients_mutex.unlock();

        }
    });
}

void CommunicationModule::initClientConnection()
{
    pUdpSocket->bind(QHostAddress::LocalHost);

    announcementThread = std::thread([&]
    {
        auto const constexpr msg = AppClientAnnouncementMsg
        {
            .msgId = APP_MSG_ID,
            .msgType = MSG_TYPE_CLIENT_ANNOUNCEMENT,
        };

        while (true)
        {
            std::this_thread::sleep_for(500ms);

            pUdpSocket->writeDatagram(
                (char const *)&msg, sizeof(msg), QHostAddress(SERVER_ADDR), SERVER_PORT);
        }

    });

    connect(this->pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::clientRcv);

}

void CommunicationModule::sndSpectogram(QVector<double> &spectogram)
{
//    qDebug() << "sending new spectogram";

    auto msg = AppDataMsg{.msgId = APP_MSG_ID, .seqNo = txSeqNo};
    memcpy(msg.data, (uint8_t *)spectogram.data(), std::min(spectogram.size(), (qsizetype)USEFUL_SPECTOGRAM_DATA_LEN) * sizeof(double));

    auto const lock = std::lock_guard(clients_mutex);
    for (auto &[k,v] : clients) {
        pUdpSocket->writeDatagram((char const *) &msg, sizeof(msg), v.hostAddr, v.hostPort);
    }

    ++txSeqNo;
}

void CommunicationModule::serverRcv()
{
    QNetworkDatagram const d = pUdpSocket->receiveDatagram();
    QByteArray arr = d.data();
    auto *p_msg = (AppClientAnnouncementMsg const *)arr.constData();

    if (APP_MSG_ID != p_msg->msgId)
    {
        // drop
    }
    else if (MSG_TYPE_CLIENT_ANNOUNCEMENT != p_msg->msgType)
    {
        // drop
    }
    else
    {
        auto const client = Client {
            .delete_flag = false,
            .hostAddr = d.senderAddress(),
            .hostPort = (uint16_t)d.senderPort()
        };

        // insert or update
        auto const lock = std::lock_guard(clients_mutex);

        auto it = clients.find({client.hostAddr.toIPv4Address(), client.hostPort});

        if (it != clients.end())
        {
            it->second.delete_flag = false;
        }
        else
        {
            clients.insert({{client.hostAddr.toIPv4Address(), client.hostPort}, client});
            emit clientConnected(clients.size(), client);
        }
    }

}

void CommunicationModule::clientRcv()
{
    QNetworkDatagram d;

    while (pUdpSocket->hasPendingDatagrams()) {
        d = pUdpSocket->receiveDatagram();
    }

    QByteArray arr = d.data();
    AppDataMsg const *p_msg = (AppDataMsg const *)arr.constData();
    //        qDebug() << "rcvd id=0x" << Qt::hex <<p_msg->msgId << " seq_no=0x" << p_msg->seqNo << " val=" << *(double *)&p_msg->data;
    emit rcvd(*p_msg);
}
