#ifndef SOUNDCOLLECTOR_H
#define SOUNDCOLLECTOR_H
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioSource>

#define RANGE_START 10
#define RANGE_END   12000
#define SAMPLE_FREQ 48000
#define FFT_BUF_SIZE (SAMPLE_FREQ / 100u)
#define USEFUL_SPECTOGRAM_DATA_LEN (FFT_BUF_SIZE / 4u)
#define NUM_SAMPLES FFT_BUF_SIZE

class SoundCollector  : public QIODevice
{
    Q_OBJECT

public:
    SoundCollector();
    void start();
    void stop();
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

signals:
    void newData(double *const p_data, uint32_t const len);

private:
    QAudioFormat format;
    std::shared_ptr<QAudioSource> pAudioSrc;

};

#endif // SOUNDCOLLECTOR_H
