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

AudioOutputWorker::AudioOutputWorker(QObject * parent)
    :   QObject(parent), m_format(QAudioDeviceInfo::defaultOutputDevice().preferredFormat()),
      m_buffer(RINGBUFFERSIZE), m_processedMs(0), m_paused(0)
{
    connect(this, &AudioOutputWorker::bufferPopulated, this, &AudioOutputWorker::push);
}

int AudioOutputWorker::deliver(const Spotinetta::AudioFrameCollection &collection)
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
        qint64 freeBytes = m_buffer.free();
        qint64 bytesPerFrame = newFormat.bytesPerFrame();
        qint64 toWrite = qMin((qint64) collection.bytes(), freeBytes);
        toWrite -= (toWrite % bytesPerFrame);

        qint64 written = m_buffer.write(collection.data(), toWrite);
        Q_ASSERT(written == toWrite);

        if (written > 0 && freeBytes == m_buffer.size())
        {
            // Signal that we've populated the buffer
            emit bufferPopulated();
        }

        // Return number of frames consumed
        consumed = toWrite / bytesPerFrame;
    }

    m_writeLock.unlock();
    return consumed;
}

bool AudioOutputWorker::isPaused() const
{
    return m_paused.load();
}

void AudioOutputWorker::pause()
{
    m_paused.store(1);
}

void AudioOutputWorker::unpause()
{
    m_paused.store(0);
    schedulePush();
}

void AudioOutputWorker::reset()
{
    // Make sure we have exclusive read and write access
    QMutexLocker writeLocker(&m_writeLock);
    QMutexLocker readLocker(&m_readLock);

    m_buffer.clear();
}

void AudioOutputWorker::schedulePush()
{
    QMetaObject::invokeMethod(this, "push", Qt::QueuedConnection);
}

void AudioOutputWorker::push()
{
    // Try to lock the read lock. If we fail, it means a reset is in progress,
    // in which case we don't want to wait around (might cause skipping in audio).
    if (!m_readLock.tryLock())
        return;

    if (isPaused())
    {
        m_readLock.unlock();
        return;
    }

    QAudioFormat format;
    {
        QMutexLocker locker(&m_formatLock);
        format = m_format;
    }

    if (!outputIsReady())
    {
        if (!setupOutput(format))
        {
            m_readLock.unlock();
            return;
        }
    }

    // If the format is correct, write to output
    if (m_output->format() == format)
    {
        if (m_buffer.used() > 0)
        {
            qint64 bytesPerFrame = format.bytesPerFrame();
            qint64 toRead = qMin(m_buffer.used(), (qint64) m_output->bytesFree());

            // Adjust toRead for frame boundaries
            toRead -= (toRead % bytesPerFrame);

            // Read to intermediate buffer. Should be the size of the output buffer,
            // and should thus be able to hold all the data
            qint64 read = m_buffer.read(m_intermediate.data(), toRead);

            // Transfer data from intermediate buffer to output device.
            qint64 written = m_device->write(m_intermediate.data(), read);

            Q_UNUSED(written)
            Q_ASSERT(read == toRead);
            Q_ASSERT(written == read);
        }
        else
        {
            // There is no data in buffer, signal
            emit bufferEmpty();
        }
    }
    // If format's not the same and we're not playing anything, recreate output
    else if (m_output->state() != QAudio::ActiveState)
    {
        setupOutput(format);
    }

    updatePosition();
    m_readLock.unlock();
}

bool AudioOutputWorker::setupOutput(const QAudioFormat &format)
{
    if (!m_output.isNull() && m_output->format() != format)
    {
        m_output->deleteLater();
        m_output.clear();
    }

    if (m_output.isNull())
    {
        m_output = new QAudioOutput(format, this);
    }

    m_output->setBufferSize(INTERNALBUFFERSIZE);
    m_device = m_output->start();

    if (m_output->error() != QAudio::NoError)
    {
        emit audioDeviceFailed();
        return false;
    }

    // Update at an interval equal to one third of the duration
    // the buffer is able to hold
    int notifyMs = format.durationForBytes(m_output->bufferSize()) / 3000;
    m_output->setNotifyInterval(notifyMs);

    // Make sure sufficient space is allocated for the intermediate buffer
    m_intermediate.resize(m_output->bufferSize());

    // Reset amount of processed milliseconds (used in position tracking)
    m_processedMs = 0;

    connect(m_output.data(), &QAudioOutput::notify, this, &AudioOutputWorker::push);
    connect(m_output.data(), &QAudioOutput::stateChanged, this, &AudioOutputWorker::onStateChanged);

    return true;
}

void AudioOutputWorker::onStateChanged(QAudio::State state)
{
    if (state == QAudio::IdleState)
    {
        // Schedule a push in the rare case that data is delivered just after
        // checking if the buffer is non-empty (in push())
        schedulePush();
    }

    Q_UNUSED(state)
}

void AudioOutputWorker::updatePosition()
{
    qint64 processedMs = m_output->processedUSecs() / 1000;
    qint64 deltaProcess = processedMs - m_processedMs;
    m_processedMs = processedMs;
    emit processed(deltaProcess);
}

bool AudioOutputWorker::outputIsReady() const
{
    return !(m_output.isNull() || m_output->error() == QAudio::FatalError || m_output->error() == QAudio::OpenError || m_output->error() == QAudio::IOError);
}

AudioOutput::AudioOutput(QObject *parent)
    :   QObject(parent)
{
    m_audioThread = new QThread(this);
    m_worker = new AudioOutputWorker;
    m_worker->moveToThread(m_audioThread);

    connect(m_worker, &AudioOutputWorker::bufferEmpty, this, &AudioOutput::bufferEmpty);
    connect(m_worker, &AudioOutputWorker::bufferPopulated, this, &AudioOutput::bufferPopulated);
    connect(m_worker, &AudioOutputWorker::audioDeviceFailed, this, &AudioOutput::audioDeviceFailed);
    connect(m_worker, &AudioOutputWorker::processed, this, &AudioOutput::onProcessed);

    m_audioThread->start();
}

AudioOutput::~AudioOutput()
{
    m_audioThread->exit();
    m_audioThread->wait();
    delete m_worker;
}

int AudioOutput::deliver(const Spotinetta::AudioFrameCollection &collection)
{
    return m_worker->deliver(collection);
}

void AudioOutput::reset(int position)
{
    reset();
    m_position = position;
    emit positionChanged();
}

bool AudioOutput::isPaused() const
{
    return m_worker->isPaused();
}

int AudioOutput::position() const
{
    return m_position;
}

void AudioOutput::pause()
{
    m_worker->pause();
    emit isPausedChanged();
}

void AudioOutput::unpause()
{
    m_worker->unpause();
    emit isPausedChanged();
}

void AudioOutput::reset()
{
    m_worker->reset();
}

void AudioOutput::onProcessed(int deltams)
{
    m_position += deltams;
    emit positionChanged();
}

}
