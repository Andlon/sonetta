#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>
#include <Spotinetta/detail/ringbuffer.h>
#include <QAudioOutput>
#include <QAtomicInt>
#include <QThread>

namespace Sonetta {

class AudioOutput;

class AudioOutputWorker : public QObject {
    Q_OBJECT
public:
    explicit AudioOutputWorker(AudioOutput * output);

public slots:
    void push();

private slots:
    void onStateChanged(QAudio::State);

private:
    void setupOutput(const QAudioFormat &format);

    QPointer<QAudioOutput>  m_output;
    QPointer<QIODevice>     m_device;
    QPointer<AudioOutput>   m_audioOutput;

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

    int position() const;
    void resetPosition(int pos);

private:
    QAudioFormat            m_format;
    QMutex                  m_formatLock;

    QThread *               m_audioThread;
    AudioOutputWorker *     m_worker;

    QAtomicInt              m_position;

    Spotinetta::detail::RingBuffer<char, 1024> m_buffer;

    friend class AudioOutputWorker;
};

}
