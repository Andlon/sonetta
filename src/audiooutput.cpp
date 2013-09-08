#include "audiooutput.h"

#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {



AudioOutput::AudioOutput(QObject *parent)
    :   QObject(parent), m_buffer(50000),
      m_deviceOffset(0), m_posOffset(0)
{

}

AudioOutput::~AudioOutput()
{
}

int AudioOutput::deliver(const Spotinetta::AudioFrameCollection &collection)
{
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

        // Replace SLOT macro way of calling with a more modern function pointer approach
        QMetaObject::invokeMethod(this, "push", Qt::QueuedConnection);

        // Return number of frames consumed
        return toWrite / bytesPerFrame;
    }

    return 0;
}

void AudioOutput::reset()
{
    qDebug() << "Reset!";
}

void AudioOutput::push()
{
    QAudioFormat format;
    {
        QMutexLocker locker(&m_formatLock);
        format = m_format;
    }

    if (m_output.isNull())
    {
        setupOutput(format);
    }

    if (m_output->format() == format)
    {
        qint64 bytesPerFrame = format.bytesPerFrame();
        qint64 toRead = qMin(m_buffer.used(), (qint64) m_output->bytesFree());

        // Adjust toRead for frame boundaries
        toRead -= (toRead % bytesPerFrame);

        QByteArray data;
        data.resize(toRead);
        qint64 read = m_buffer.read(data.data(), data.size());
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

    emit notify();
}

void AudioOutput::setupOutput(const QAudioFormat &format)
{
    m_output = new QAudioOutput(format, this);
    m_device = m_output->start();
    m_deviceOffset = 0;

    int notifyMs = format.durationForBytes(m_output->bufferSize() / 2) / 1000;
    m_output->setNotifyInterval(notifyMs);

    connect(m_output, &QAudioOutput::notify, this, &AudioOutput::push);
}

void AudioOutput::resetPosition(int pos)
{
    m_deviceOffset = m_output.isNull() ? 0 : m_output->elapsedUSecs() / 1000;
    m_posOffset = pos;
}

int AudioOutput::position() const
{
    if (!m_output.isNull())
    {
        int devPos = m_output->elapsedUSecs() / 1000;
        return devPos - m_deviceOffset + m_posOffset;
    }
    else
    {
        return 0;
    }
}

}
