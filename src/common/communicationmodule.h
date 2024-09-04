#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <QUdpSocket>
#include <QHostAddress>
#include <memory>
#include <map>
#include <tuple>
#include <mutex>
#include "server/soundcollector.h"

/**
 * @brief Client data structure
 */
struct Client
{
    bool delete_flag;      /**< Flag indicating whether client should be deleted on the next check */
    QHostAddress hostAddr; /**< Client IP address */
    uint16_t hostPort;     /**< Client address */
};

/**
 * @brief Enumeration of available message types
 */
enum class EMsgType {
    DATA,
    CLIENT_ANNOUNCEMENT
};

__pragma(pack(push, 1));
/**
 * @brief Message header structure
 */
struct MsgHdr
{
    uint32_t msgId;     /**< Message identification value */
    uint8_t msgType;    /**< Message type (EMsgType) */
};

/**
 * @brief Audio data message structure sent from server to client
 */
struct MsgAudioData
{
    MsgHdr hdr;                             /**< Message header */
    uint32_t seqNo;                         /**< Message sequence number */
    double data[USEFUL_SPECTOGRAM_DATA_LEN];/**< Audio data */
};

/**
 * @brief Client announcement message sent from client to server
 */
struct MsgClientAnnouncement
{
    MsgHdr hdr;         /**< Message header */
};
__pragma(pack(pop));

/**
 * @brief Communication module for communicating with server or client
 */
class CommunicationModule : public QObject
{
    Q_OBJECT

public:
    auto const constexpr static APP_MSG_ID = 0xAA00DD11u;           /**< Application message identification constant */
    auto const constexpr static DEFAULT_SERVER_PORT = 55123;        /**< Default server port */
    auto const constexpr static DEFAULT_SERVER_ADDR = "127.0.0.1";  /**< Default server address */
    auto const constexpr static CONN_TMT_MS = 1000u;                /**< Connection timeout in milliseconds */


    /**
     * @brief CommunicationModule constructor
     */
    CommunicationModule();

    /**
     * @brief Function initializes server connection to clients (for listening to clients
     *        announcement messages and audio data sending)
     * @param addr Address on which the server will be listening on
     * @param port Port on which the server will be listetning on
     */
    void initServerConnection(QHostAddress addr, uint16_t port);

    /**
     * @brief Function initializes client connection to server (for listening to servers audio data
     *        and sending clinet announcement messages)
     * @param addr Address of the server to which client has to connect
     * @param port Port of the server to which client has to connect
     */
    void initClientConnection(QHostAddress addr, uint16_t port);

    /**
     * @brief Function sends audio data message to connected clients
     * @param spectogram
     */
    void sendAudioData(QVector<double> &audioData);

    /**
     * @brief Fucntion stops communication threads and disconnects Qt signals
     */
    void stop();

public slots:
    /**
     * @brief Slot handling receving messages on the server side
     */
    void serverRcv();
    /**
     * @brief Slot handling receiving messages on the client side
     */
    void clientRcv();

signals:
    /**
     * @brief New audio data received event signal
     * @param data Audio data message reference
     */
    void audioDataRcvd(MsgAudioData const &data);

    /**
     * @brief Client connected / disconnected event signal
     * @param client_no New number of connected clients
     * @param newClient Reference to newly connected / disconenceted client
     */
    void clientConnectionChanged(uint32_t client_no, Client const &newClient);

    /**
     * @brief Client connected to server event signal
     */
    void serverConnected();

    /**
     * @brief Client lost connection with server event signal
     */
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
