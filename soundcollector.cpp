#include "soundcollector.h"
#include <QMediaDevices>

SoundCollector::SoundCollector()
{
    this->format.setSampleRate(SAMPLE_FREQ);
    this->format.setChannelCount(1);
    this->format.setSampleFormat(QAudioFormat::Int16);
    this->pAudioSrc = std::make_shared<QAudioSource>(QMediaDevices::defaultAudioInput(), this->format);
    this->pAudioSrc->start(this);
    this->start();
}

void SoundCollector::start()
{
    this->open(QIODevice::WriteOnly);
}

void SoundCollector::stop()
{
    this->close();
}

qint64 SoundCollector::readData(char *data, qint64 maxlen)
{
    return 0;
}

qint64 SoundCollector::writeData(const char *data, qint64 len)
{
    auto const numSamples = len / format.bytesPerFrame();
    auto p_result = (double *)new double[numSamples];

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < format.channelCount(); ++j) {
            p_result[i] = (double)format.normalizedSampleValue(data);
            data += format.bytesPerSample();
        }
    }

    emit newData(p_result, numSamples);
    delete[] p_result;
    return len;
}
