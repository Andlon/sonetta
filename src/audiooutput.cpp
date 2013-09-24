#include "audiooutput.h"

#include <QDebug>
#include <QMutexLocker>

namespace sp = Spotinetta;

namespace Sonetta {

AudioOutputWorker::AudioOutputWorker(AudioOutput *output)
    :   QObject(nullptr), m_audioOutput(output), m_processedMs(0)
{

}

void AudioOutputWorker::push()
{
    AudioOutput * o = m_audioOutput;
    QAudioFormat format;
    {
        QMutexLocker locker(&o->m_formatLock);
        format = o->m_format;
    }

    if (m_output.isNull())
    {
        setupOutput(format);
    }

    if (m_output->format() == format)
    {
        qint64 bytesPerFrame = format.bytesPerFrame();
        qint64 toRead = qMin(o->m_buffer.used(), (qint64) m_output->bytesFree());

        // Adjust toRead for frame boundaries
        toRead -= (toRead % bytesPerFrame);

        QByteArray data;
        data.resize(toRead);
        qint64 read = o->m_buffer.read(data.data(), data.size());
        qint64 written = m_device->write(data);

        Q_ASSERT(read == toRead);
        Q_ASSERT(written == read);
    }
    // Will this trigger on buffer underruns? Investigate
    else if (m_output->state() != QAudio::ActiveState)
    {
        m_output->deleteLater();
        m_output.clear();
        setupOutput(format);
    }

    // Update position with the amount of processed milliseconds
    // since the last time this function was called
    int processedMs = m_output->processedUSecs() / 1000;
    int deltaProcess = processedMs - m_processedMs;
    m_processedMs = processedMs;

    m_audioOutput->m_position.fetchAndAddOrdered(deltaProcess);
}

void AudioOutputWorker::setupOutput(const QAudioFormat &format)
{
    m_output = new QAudioOutput(format, this);

    m_output->setBufferSize(70000);
    m_device = m_output->start();

    int notifyMs = format.durationForBytes(m_output->bufferSize() / 2) / 1000;
    m_output->setNotifyInterval(notifyMs);

    // Reset amount of processed milliseconds (used in position tracking)
    m_processedMs = 0;

    connect(m_output.data(), &QAudioOutput::notify, this, &AudioOutputWorker::push);
    connect(m_output.data(), &QAudioOutput::stateChanged, this, &AudioOutputWorker::onStateChanged);
}

void AudioOutputWorker::onStateChanged(QAudio::State state)
{
    qDebug() << "Audio state: " << state;
    qDebug() << "Audio error: " << m_output->error();
}

AudioOutput::AudioOutput(QObject *parent)
    :   QObject(parent), m_buffer(50000)
{
    m_position.store(0);

    m_audioThread = new QThread(this);
    m_worker = new AudioOutputWorker(this);
    m_worker->moveToThread(m_audioThread);

    connect(m_audioThread, &QThread::finished,
            m_worker, &AudioOutputWorker::deleteLater);

    m_audioThread->start();
}

AudioOutput::~AudioOutput()
{
    m_audioThread->exit();
    m_audioThread->wait();
}

int AudioOutput::deliver(const Spotinetta::AudioFrameCollection &collection)
{
    // If tryLock fails, a reset is in progress, in which case we don't want to
    // try to write anything, as it will only block
    if (!m_writeLock.tryLock())
        return 0;

    int consumed = 0;
    sp::AudioFormat newFormat = collection.format();

    bool formatsEqual;
    {
        QMutexLocker locker(&m_formatLock);
        formatsEqual = newFormat == m_format;

        if (!formatsEqual && m_buffer.used() == 0)
        {
            m_format = newFormat;
        }
    }

    if (formatsEqual)
    {
        qint64 bytesPerFrame = newFormat.bytesPerFrame();
        qint64 toWrite = qMin(collection.bytes(), (int) m_buffer.free());
        toWrite -= (toWrite % bytesPerFrame);

        qint64 written = m_buffer.write(collection.data(), toWrite);
        Q_ASSERT(written == toWrite);

        QMetaObject::invokeMethod(m_worker, "push", Qt::QueuedConnection);

        // Return number of frames consumed
        consumed = toWrite / bytesPerFrame;
    }

    m_writeLock.unlock();
    return consumed;
}

void AudioOutput::reset()
{
    // Make sure we have exclusive write access (avoids blocking Spotify's music
    // thread while resetting)
    QMutexLocker locker(&m_writeLock);

    // Make sure we're actually pulling data before clearing (since it blocks
    // until the buffer is empty before resetting).
    QMetaObject::invokeMethod(m_worker, "push", Qt::QueuedConnection);

    // Note: investigate _possible_ race condition here. Is it possible that
    // clear will block while waiting for the buffer data to be read while
    // push is not actually being called? If so, it will block indefinitely.
    m_buffer.clear();
}

void AudioOutput::resetPosition(int pos)
{
    m_position.store(pos);
}

int AudioOutput::position() const
{
    return m_position.load();
}

}
