#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>
#include <Spotinetta/detail/ringbuffer.h>
#include <QAudioOutput>
#include <QAtomicInt>
#include <QThread>
#include <QMutex>

namespace Sonetta {

class AudioOutput;

class AudioOutputWorker : public QObject {
    Q_OBJECT
public:
    explicit AudioOutputWorker(AudioOutput * output);

public slots:
    void push();

signals:
    void bufferEmpty();

private slots:
    void onStateChanged(QAudio::State);

private:
    void setupOutput(const QAudioFormat &format);

    QPointer<QAudioOutput>  m_output;
    QPointer<QIODevice>     m_device;
    QPointer<AudioOutput>   m_audioOutput;

    QByteArray              m_intermediate;
    int                     m_processedMs;
};

class AudioOutput : public QObject, public Spotinetta::AudioOutputInterface
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject * parent = 0);
    ~AudioOutput();
    int deliver(const Spotinetta::AudioFrameCollection &collection);
    void reset();

    bool isPlaying() const;

    int position() const;
    void resetPosition(int pos);

    void start();
    void stop();

signals:
    void bufferEmpty();
    void bufferPopulated();

private:
    QAudioFormat            m_format;
    QMutex                  m_formatLock;
    QMutex                  m_writeLock;
    mutable QMutex          m_readLock;

    QThread *               m_audioThread;
    AudioOutputWorker *     m_worker;

    QAtomicInt              m_position;
    bool                    m_paused;

    Spotinetta::detail::RingBuffer<char, 1024> m_buffer;

    friend class AudioOutputWorker;
};

}
