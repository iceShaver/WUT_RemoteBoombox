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

/**
 * @brief Sound collector
 */
class SoundCollector  : public QIODevice
{
    Q_OBJECT

public:
    /**
     * @brief SoundCollector constructor
     */
    SoundCollector();

    /**
     * @brief SoundCollector desctructor
     * @param dev
     */
    SoundCollector(QAudioDevice dev);

    /**
     * @brief Function starts sound collector with defined settings
     */
    void start();

    /**
     * @brief Function stops sound collector operation
     */
    void stop();

    /**
     * @brief Mock function for QIODevice data read (device perspective, unused but had to be defined)
     * @param data Pointer to data to read
     * @param maxlen Length of data to read
     * @return
     */
    qint64 readData(char *data, qint64 maxlen) override;

    /**
     * @brief Function for QIODevice data write 9device perspective)
     * @param data Pointer to the new audio data
     * @param len Lenght of audio data
     * @return Length of audio data
     */
    qint64 writeData(const char *data, qint64 len) override;

    /**
     * @brief Static function which returns available audio inputs
     * @return List of available audio inputs
     */
    static QList<QAudioDevice> getInputs();

signals:
    /**
     * @brief newData New audio data signal
     * @param p_data Pointer to the new audio data
     * @param len Length of the new audio data
     */
    void newData(double *const p_data, uint32_t const len);

private:
    QAudioFormat format;
    std::unique_ptr<QAudioSource> pAudioSrc;

};

#endif // SOUNDCOLLECTOR_H
