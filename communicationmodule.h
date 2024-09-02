#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <QUdpSocket>
#include <QHostAddress>
#include <memory>
#include <map>
#include <tuple>
#include <mutex>
#include "soundcollector.h"


struct Client
{
    bool delete_flag;
    QHostAddress hostAddr;
    uint16_t hostPort;
};

class CommunicationModule : public QObject
{
    Q_OBJECT

public:
    auto const constexpr static APP_MSG_ID = 0xAA00DD11u;
    auto const constexpr static MSG_TYPE_DATA = 0x55u;
    auto const constexpr static MSG_TYPE_CLIENT_ANNOUNCEMENT = 0xEFu;
    auto const constexpr static SERVER_PORT = 55123;
    auto const constexpr static SERVER_ADDR = "127.0.0.1";
    auto const constexpr static CLIENT_TMT_MS = 1000u;

    using AudioDataType = double;

    __pragma(pack(push, 1))
    struct AppDataMsg
    {
        uint32_t msgId;
        uint8_t msgType;
        uint32_t seqNo;
        double data[USEFUL_SPECTOGRAM_DATA_LEN];
    };

    struct AppClientAnnouncementMsg
    {
        uint32_t msgId;
        uint8_t msgType;
    };

    __pragma(pack(pop))



    CommunicationModule();
    void initServerConnection();
    void initClientConnection();
    void sndSpectogram(QVector<double> &spectogram);
    void serverRcv();
    void clientRcv();

signals:
    void rcvd(AppDataMsg const data);
    void clientConnected(uint32_t client_no, Client const &newClient);


private:
    std::unique_ptr<QUdpSocket> pUdpSocket;
    uint32_t txSeqNo;
    uint32_t rxSeqNo;

    std::map<std::tuple<uint32_t, uint16_t>, Client> clients;
    std::mutex clients_mutex;
    std::thread clientManagementThread;
    std::thread announcementThread;
};

#endif // COMMUNICATIONMODULE_H
