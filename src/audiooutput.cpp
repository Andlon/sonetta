#include "audiooutput.h"

#include <QDebug>
#include <QMutexLocker>

namespace sp = Spotinetta;

namespace {
// By keeping the ring buffer relatively large and the internal buffer of the output
// relatively small, we should be able to achieve buttery smooth playback while maintaining
// a high degree of responsiveness in track changes, seek etc.
const qint64 RINGBUFFERSIZE = 500000;
const qint64 INTERNALBUFFERSIZE = 50000;
}

namespace Sonetta {

AudioOutputWorker::AudioOutputWorker(AudioOutput *output)
    :   QObject(nullptr), m_audioOutput(output), m_processedMs(0)
{

}

void AudioOutputWorker::push()
{
    AudioOutput * o = m_audioOutput;

    if (o->m_paused)
        return;

    // Try to lock the read lock. If we fail, it means a reset is in progress,
    // in which case we don't want to wait around (might cause skipping in audio).
    if (!o->m_readLock.tryLock())
        return;

    QAudioFormat format;
    {
        QMutexLocker locker(&o->m_formatLock);
        format = o->m_format;
    }

    if (m_output.isNull())
    {
        setupOutput(format);
    }

    // If the format is correct, write to output
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
    // If format's not the same, recreate output
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

    o->m_readLock.unlock();
}

void AudioOutputWorker::setupOutput(const QAudioFormat &format)
{
    m_output = new QAudioOutput(format, this);

    m_output->setBufferSize(INTERNALBUFFERSIZE);
    m_device = m_output->start();

    // Update at an interval equal to one third of the duration
    // the buffer is able to hold
    int notifyMs = format.durationForBytes(m_output->bufferSize()) / 3000;
    m_output->setNotifyInterval(notifyMs);

    // Reset amount of processed milliseconds (used in position tracking)
    m_processedMs = 0;

    connect(m_output.data(), &QAudioOutput::notify, this, &AudioOutputWorker::push);
    connect(m_output.data(), &QAudioOutput::stateChanged, this, &AudioOutputWorker::onStateChanged);
}

void AudioOutputWorker::onStateChanged(QAudio::State state)
{
    if (state == QAudio::ActiveState)
    {
        emit started();
    }
    else
    {
        emit stopped();
    }
}

AudioOutput::AudioOutput(QObject *parent)
    :   QObject(parent), m_buffer(RINGBUFFERSIZE), m_paused(false)
{
    m_position.store(0);

    m_audioThread = new QThread(this);
    m_worker = new AudioOutputWorker(this);
    m_worker->moveToThread(m_audioThread);

    connect(m_audioThread, &QThread::finished,
            m_worker, &AudioOutputWorker::deleteLater);
    connect(m_worker, &AudioOutputWorker::started, this, &AudioOutput::started);
    connect(m_worker, &AudioOutputWorker::stopped, this, &AudioOutput::stopped);

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
    // try to write anything, as it will only block Spotify's music thread
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
            // Only change the current format when the buffer is empty.
            // Consequently, we'll only ever write data of a different
            // format when there is no existing data (of any other format).
            // As a result, transition between different formats is smooth.
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
    // Make sure we have exclusive read and write access
    QMutexLocker writeLocker(&m_writeLock);
    QMutexLocker readLocker(&m_readLock);

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

void AudioOutput::start()
{
    QMutexLocker readLocker(&m_readLock);
    m_paused = false;
}

void AudioOutput::stop()
{
    QMutexLocker readLocker(&m_readLock);
    m_paused = true;
}

}
