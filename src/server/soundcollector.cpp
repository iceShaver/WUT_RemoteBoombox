#include <QMediaDevices>
#include <QDebug>
#include "soundcollector.h"

SoundCollector::SoundCollector() : SoundCollector(QMediaDevices::defaultAudioInput())
{
}

SoundCollector::SoundCollector(QAudioDevice dev)
{
    this->format.setSampleRate(SAMPLE_FREQ);
    this->format.setChannelCount(1);
    this->format.setSampleFormat(QAudioFormat::Int16);

    this->pAudioSrc = std::make_unique<QAudioSource>(dev, this->format);
    this->start();
    this->pAudioSrc->start(this);
}

void SoundCollector::start()
{
    if (this->pAudioSrc)
    {
        this->open(QIODevice::WriteOnly);
        this->pAudioSrc->start(this);
    }

}

void SoundCollector::stop()
{
    if (this->pAudioSrc)
    {
        this->pAudioSrc->stop();
        this->close();
    }
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

QList<QAudioDevice> SoundCollector::getInputs()
{
    return QMediaDevices::audioInputs();
}
