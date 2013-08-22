#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QIODevice>
#include <QSemaphore>
#include <QMutex>
#include <QAtomicInt>
#include <QReadWriteLock>

#include <QAudioFormat>

class AudioFrameCollection {
public:
    AudioFrameCollection(const void * frames, int frameCount, const QAudioFormat &format);

    const void * frames() const;
    QAudioFormat format() const;

    int frameCount() const;

private:
    int m_frameCount;
    const void * m_frames;
    QAudioFormat m_format;
};

class CircularAudioBuffer : public QIODevice
{
public:
    CircularAudioBuffer(int bufferSize, int chunkSize, QObject * parent = 0);
    ~CircularAudioBuffer();

    qint64 bytesAvailable() const;

    bool isSequential() const { return true; }

    bool reset();

    bool open(OpenMode mode);
    void close();

    int bufferSize() const;
    int chunkSize() const;
    QAudioFormat format() const;

    void setBufferSize(int frameCount);
    void setChunkSize(int frameCount);
    void setFormat(const QAudioFormat &format);

    int writeFrames(const AudioFrameCollection &frames);

protected:
    qint64 writeData(const char *data, qint64 len);
    qint64 readData(char *data, qint64 maxlen);

private:
    char * m_buffer;

    int m_start;
    int m_end;
    QSemaphore m_free;
    QSemaphore m_used;

    QAtomicInt m_chunkSize; // In frames
    QAtomicInt m_bufferSize; // In frames

    QMutex m_writeMutex;
    QMutex m_readMutex;
    mutable QReadWriteLock m_formatLock;

    QAudioFormat m_format;
};

#endif // CIRCULARBUFFER_H
