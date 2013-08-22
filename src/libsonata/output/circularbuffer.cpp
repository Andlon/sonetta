#include "circularbuffer.h"
#include <QDebug>
#include <QMutexLocker>
#include <QReadLocker>
#include <QWriteLocker>

AudioFrameCollection::AudioFrameCollection(const void *frames, int frameCount, const QAudioFormat &format)
    :   m_frameCount(frameCount), m_frames(frames), m_format(format)
{

}

const void * AudioFrameCollection::frames() const
{
    return m_frames;
}

QAudioFormat AudioFrameCollection::format() const
{
    return m_format;
}

int AudioFrameCollection::frameCount() const
{
    return m_frameCount;
}

CircularAudioBuffer::CircularAudioBuffer(int bufferSize, int chunkSize, QObject * parent)
    :   QIODevice(parent), m_start(0), m_end(0), m_chunkSize(chunkSize), m_bufferSize(bufferSize)
{
    Q_ASSERT(bufferSize > 0);
    Q_ASSERT(chunkSize > 0);

    // Default format used by Spotify
    m_format.setSampleRate(44100);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setChannelCount(2);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
}

CircularAudioBuffer::~CircularAudioBuffer()
{
    if (isOpen())
        close();
}

qint64 CircularAudioBuffer::bytesAvailable() const
{
    QReadLocker formatLocker(&m_formatLock);
    int bytesPerFrame = m_format.bytesPerFrame();
    return m_free.available() * bytesPerFrame + QIODevice::bytesAvailable();
}

qint64 CircularAudioBuffer::writeData(const char *data, qint64 len)
{
    qint64 bytesWritten = 0;

    QMutexLocker writeLocker(&m_writeMutex);

    while (bytesWritten < len)
    {
        if (m_used.available() == 0)
        {
            // Buffer full, return
            return bytesWritten;
        }

        qint64 pos = m_end;

        m_formatLock.lockForRead();
        const int frameSize = m_format.bytesPerFrame();
        m_formatLock.unlock();
        int framesLeft = (len - bytesWritten) / frameSize;
        int acquire = qMin<int>(m_chunkSize.load(), framesLeft);

        // If there is not enough space, we don't want to write to avoid locking the thread
        if (acquire == 0 || m_used.available() < acquire)
            return bytesWritten;

        m_used.acquire(acquire);

        qint64 bufferByteSize = m_bufferSize.load() * frameSize;

        for (int i = 0; i < acquire; ++i)
        {
            // Loop through frames
            for (int j = 0; j < frameSize; ++j)
            {
                // Loop through bytes per frame
                const qint64 bytePos = i * frameSize + j;
                *(m_buffer + ((pos + bytePos) % bufferByteSize)) = *(data + bytesWritten);
                ++bytesWritten;
            }
        }

        m_free.release(acquire);
        m_end = (m_end + acquire * frameSize) % bufferByteSize;

    }

    return bytesWritten;
}

int CircularAudioBuffer::writeFrames(const AudioFrameCollection &audioFrames)
{
    QAudioFormat newFormat = audioFrames.format();
    if (format() != newFormat)
    {
        QMutexLocker writeLocker(&m_writeMutex);
        QMutexLocker readLocker(&m_readMutex);
        if (bytesAvailable() == 0)
        {
            // Switch format
            m_formatLock.lockForWrite();
            m_format = newFormat;
            m_formatLock.unlock();

            // Adapt buffer
            delete[] m_buffer;
            m_buffer = new char[m_bufferSize.load() * newFormat.bytesPerFrame()];

            // Reset start and end counters
            m_start = m_end = 0;

            const char * data = static_cast<const char *>(audioFrames.frames());
            qint64 byteCount = newFormat.bytesPerFrame() * audioFrames.frameCount();

            writeLocker.unlock();
            readLocker.unlock();
            return write(data, byteCount) / newFormat.bytesPerFrame();
        }
        return 0;
    }
    else
    {
        const char * data = static_cast<const char *>(audioFrames.frames());
        qint64 byteCount = newFormat.bytesPerFrame() * audioFrames.frameCount();
        qint64 written = write(data, byteCount);

        Q_ASSERT(written % newFormat.bytesPerFrame() == 0);

        return written / newFormat.bytesPerFrame();
    }
}

qint64 CircularAudioBuffer::readData(char *data, qint64 maxlen)
{
    qint64 bytesRead = 0;

    QMutexLocker readLocker(&m_readMutex);

    while (bytesRead < maxlen)
    {
        if (m_free.available() == 0)
        {
            // Buffer is empty
            return bytesRead;
        }

        qint64 pos = m_start;

        m_formatLock.lockForRead();
        const int frameSize = m_format.bytesPerFrame();
        m_formatLock.unlock();

        // Read chunk
        int framesLeft = (maxlen - bytesRead) / frameSize;
        int acquire = qMin<int>(framesLeft, m_chunkSize.load());
        acquire = qMin<int>(acquire, m_free.available());
        m_free.acquire(acquire);

        qint64 bufferByteSize = m_bufferSize.load() * frameSize;

        for (int i = 0; i < acquire; ++i)
        {
            for (int j = 0; j < frameSize; ++j)
            {
                qint64 bytePos = i * frameSize + j;
                *(data + bytesRead) = *(m_buffer + ((pos + bytePos) % bufferByteSize));
                ++bytesRead;
            }
        }

        m_used.release(acquire);
        m_start = (m_start + acquire * frameSize) % bufferByteSize;
    }

    return bytesRead;
}

bool CircularAudioBuffer::reset()
{
    QMutexLocker readLocker(&m_readMutex);
    QMutexLocker writeLocker(&m_writeMutex);

    m_used.release(m_free.available());
    m_free.acquire(m_free.available());

    m_start = 0;
    m_end = 0;

    return true;
}

int CircularAudioBuffer::bufferSize() const
{
    return m_bufferSize.load();
}

int CircularAudioBuffer::chunkSize() const
{
    return m_chunkSize.load();
}

QAudioFormat CircularAudioBuffer::format() const
{
    QReadLocker locker(&m_formatLock);
    return m_format;
}

void CircularAudioBuffer::setBufferSize(int frameCount)
{
    if (isOpen())
        return;

    m_bufferSize.store(frameCount);
}

void CircularAudioBuffer::setChunkSize(int frameCount)
{
    if (isOpen())
        return;

    m_chunkSize.store(frameCount);
}

bool CircularAudioBuffer::open(OpenMode mode)
{
    if (isOpen())
        return false;

    int bytesPerFrame = format().bytesPerFrame();

    m_buffer = new char[m_bufferSize.load() * bytesPerFrame];

    // Create  and acquire resources
    m_used.release(m_bufferSize.load());
    m_free.release(m_bufferSize.load());
    m_free.acquire(m_bufferSize.load());

    // Reset start and end counters
    m_start = m_end = 0;

    return QIODevice::open(mode);
}

void CircularAudioBuffer::close()
{
    QMutexLocker writeLocker(&m_writeMutex);
    QMutexLocker readLocker(&m_readMutex);

    // Reset resources
    m_used.release(m_free.available());
    m_free.acquire(m_free.available());

    // Reset start and end counters
    m_start = m_end = 0;

    // Deallocate buffer
    delete[] m_buffer;

    QIODevice::close();
}
