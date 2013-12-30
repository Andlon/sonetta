#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>
#include <Spotinetta/detail/ringbuffer.h>
#include <QAudioOutput>
#include <QAtomicInt>
#include <QThread>
#include <QMutex>

#include "utilities/pointers.h"

namespace Sonetta {

class AudioOutput;

class AudioOutputWorker : public QObject {
    Q_OBJECT
public:
    explicit AudioOutputWorker(QObject * parent = 0);

    int deliver(const Spotinetta::AudioFrameCollection &collection);

    bool isPaused() const;
    void pause();
    void unpause();
    void reset();
    void schedulePush();

public slots:
    void push();

signals:
    void bufferEmpty();
    void bufferPopulated();
    void audioDeviceFailed();
    void processed(int deltams);

private slots:
    void onStateChanged(QAudio::State);

private:
    void updatePosition();
    bool outputIsReady() const;

    bool setupOutput(const QAudioFormat &format);

    QAudioFormat            m_format;
    QMutex                  m_formatLock;
    QMutex                  m_writeLock;
    mutable QMutex          m_readLock;

    QPointer<QAudioOutput>  m_output;
    QPointer<QIODevice>     m_device;

    Spotinetta::detail::RingBuffer<char, 1024> m_buffer;

    QByteArray              m_intermediate;
    qint64                  m_processedMs;
    QAtomicInt              m_paused;
};

class AudioOutput : public QObject, public Spotinetta::AudioOutputInterface
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject * parent = 0);
    ~AudioOutput();

    int deliver(const Spotinetta::AudioFrameCollection &collection);

    int position() const;

    bool isPaused() const;
    void pause();
    void unpause();
    void reset();
    void reset(int position);

signals:
    void bufferEmpty();
    void bufferPopulated();
    void isPausedChanged();
    void positionChanged();
    void audioDeviceFailed();

private slots:
    void onProcessed(int deltams);

private:
    ObjectScopedPointer<QThread>            m_audioThread;
    ObjectScopedPointer<AudioOutputWorker>  m_worker;

    int m_position;

    friend class AudioOutputWorker;
};

}
