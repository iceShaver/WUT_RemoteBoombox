#include <QDebug>
#include <QNetworkDatagram>
#include "communicationmodule.h"
#include <cstring>
#include <thread>
#include <format>

using namespace std::literals;

CommunicationModule::CommunicationModule()
{
    serverConnTmtCntr = 0;
}

void CommunicationModule::initServerConnection(QHostAddress addr, uint16_t port)
{
    if (pUdpSocket)
    {
        disconnect(pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::serverRcv);
    }

    pUdpSocket = std::make_unique<QUdpSocket>();

    if (false == pUdpSocket->bind(addr, port))
    {
        throw std::runtime_error(
            QString("Unable to bind to addr: %1 port: %2").arg(addr.toString().trimmed(), QString::number(port)).toStdString()
        );
    }

    connect(pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::serverRcv);

    if (clientManagementThread.joinable())
    {
        server_thread_active = false;
        clientManagementThread.join();
    }

    server_thread_active = true;

    clientManagementThread = std::thread([&]
    {
        while (server_thread_active)
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
                    emit clientConnectionChanged((uint32_t)clients.size() - 1u, it->second);
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

void CommunicationModule::initClientConnection(QHostAddress addr, uint16_t port)
{
    pUdpSocket = std::make_unique<QUdpSocket>();
    serverAddr = addr;
    serverPort = port;
    pUdpSocket->bind(QHostAddress::LocalHost);
    serverConnTmtCntr = 0;

    if (announcementThread.joinable())
    {
        client_thread_active = false;
        announcementThread.join();
    }

    client_thread_active = true;

    announcementThread = std::thread([&]
    {
        auto const constexpr msg = MsgClientAnnouncement
        {
            .hdr =
            {
                .msgId = APP_MSG_ID,
                .msgType = (uint8_t)EMsgType::CLIENT_ANNOUNCEMENT
            }
        };

        while (client_thread_active)
        {
            if (serverConnTmtCntr > 0)
            {
                serverConnTmtCntr -= 500;
                if (serverConnTmtCntr <= 0)
                {
                    emit serverDisconnected();
                }
            }

            pUdpSocket->writeDatagram(
                (char const *)&msg, sizeof(msg), serverAddr, serverPort);

            std::this_thread::sleep_for(500ms);
        }

    });

    connect(pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::clientRcv);
}

void CommunicationModule::sendAudioData(QVector<double> &audioData)
{
    auto msg = MsgAudioData
    {
        .hdr =
        {
            .msgId = APP_MSG_ID,
            .msgType = (uint8_t)EMsgType::DATA
        },
        .seqNo = txSeqNo
    };

    memcpy(msg.data, (uint8_t *)audioData.data(), USEFUL_SPECTOGRAM_DATA_LEN * sizeof(double));

    auto const lock = std::lock_guard(clients_mutex);

    for (auto &[k,v] : clients)
    {
        pUdpSocket->writeDatagram((char const *) &msg, sizeof(msg), v.hostAddr, v.hostPort);
    }

    ++txSeqNo;
}

void CommunicationModule::stop()
{
    client_thread_active = false;
    server_thread_active = false;
    serverConnTmtCntr = 0;

    if (clientManagementThread.joinable())
    {
        clientManagementThread.join();
    }

    if(announcementThread.joinable())
    {
        announcementThread.join();
    }

    if (pUdpSocket)
    {
        disconnect(pUdpSocket.get(), &QUdpSocket::readyRead, this, &CommunicationModule::serverRcv);
    }

    pUdpSocket = nullptr;

}

void CommunicationModule::serverRcv()
{
    QNetworkDatagram const d = pUdpSocket->receiveDatagram();
    QByteArray arr = d.data();
    auto *p_msg = (MsgClientAnnouncement const *)arr.constData();

    if (APP_MSG_ID != p_msg->hdr.msgId)
    {
        // drop
    }
    else if ((uint8_t)EMsgType::CLIENT_ANNOUNCEMENT != p_msg->hdr.msgType)
    {
        // drop
    }
    else
    {
        auto const client = Client
        {
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
            emit clientConnectionChanged((uint32_t)clients.size(), client);
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
    auto const *p_msg = (MsgAudioData const *)arr.constData();
    //        qDebug() << "rcvd id=0x" << Qt::hex <<p_msg->msgId << " seq_no=0x" << p_msg->seqNo << " val=" << *(double *)&p_msg->data;

    if (APP_MSG_ID != p_msg->hdr.msgId)
    {
        // drop
    }
    else if ((uint8_t)EMsgType::DATA != p_msg->hdr.msgType)
    {
        // drop
    }
    else
    {
        if (serverConnTmtCntr <= 0)
        {
            emit serverConnected();
        }

        serverConnTmtCntr = CONN_TMT_MS;
        emit audioDataRcvd(*p_msg);
    }
}

