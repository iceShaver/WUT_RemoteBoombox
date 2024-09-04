#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <QUdpSocket>
#include <QHostAddress>
#include <memory>
#include <map>
#include <tuple>
#include <mutex>
#include "server/soundcollector.h"


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
    auto const constexpr static DEFAULT_SERVER_PORT = 55123;
    auto const constexpr static DEFAULT_SERVER_ADDR = "127.0.0.1";
    auto const constexpr static CONN_TMT_MS = 1000u;

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
    void initServerConnection(QHostAddress addr, uint16_t port);
    void initClientConnection(QHostAddress addr, uint16_t port);
    void sndSpectogram(QVector<double> &spectogram);
    void serverRcv();
    void clientRcv();
    void stop();
    bool isServerConnected();

signals:
    void rcvd(AppDataMsg const data);
    void clientConnectionChanged(uint32_t client_no, Client const &newClient);
    void serverConnected();
    void serverDisconnected();

private:
    std::unique_ptr<QUdpSocket> pUdpSocket = nullptr;
    uint32_t txSeqNo = 0;
    uint32_t rxSeqNo = 0;

    std::map<std::tuple<uint32_t, uint16_t>, Client> clients;
    std::mutex clients_mutex;
    std::thread clientManagementThread;
    std::thread announcementThread;
    int32_t serverConnTmtCntr = 0;
    QHostAddress serverAddr;
    uint16_t serverPort;
    std::atomic_bool client_thread_active = true;
    std::atomic_bool server_thread_active = true;
};

#endif // COMMUNICATIONMODULE_H
