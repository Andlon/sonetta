#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>
#include <Spotinetta/detail/ringbuffer.h>
#include <QAudioOutput>
#include <QAtomicInt>

namespace Sonetta {

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

signals:
    void notify();

private slots:
    void push();

private:
    void setupOutput(const QAudioFormat &format);

    QPointer<QAudioOutput>  m_output;
    QPointer<QIODevice>     m_device;
    QAudioFormat            m_format;
    QMutex                  m_formatLock;

    int                     m_deviceOffset;
    int                     m_posOffset;

    Spotinetta::detail::RingBuffer<char, 1024> m_buffer;
};

}
