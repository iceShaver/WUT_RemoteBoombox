#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <QUdpSocket>
#include <memory>
#include "soundcollector.h"

class CommunicationModule : public QObject
{
    Q_OBJECT

public:
    auto const constexpr static DATA_MSG_ID = 0xAA00DD11u;
    auto const constexpr static PORT = 55123;
    auto const constexpr static ADDR = "127.0.0.1";

    using AudioDataType = double;

    __pragma(pack(push, 1))
    struct AppDataMsg
    {
        uint32_t msgId;
        uint32_t seqNo;
        double data[USEFUL_SPECTOGRAM_DATA_LEN];
    };
    __pragma(pack(pop))

    CommunicationModule();
    void snd(AudioDataType const &audioData);
    void sndSpectogram(QVector<double> &spectogram);
    void beginRcv(void);
    void rcv();

signals:
    void rcvd(AppDataMsg const data);

private:
    std::unique_ptr<QUdpSocket> pUdpSocket;
    uint32_t txSeqNo;
    uint32_t rxSeqNo;
};

#endif // COMMUNICATIONMODULE_H
