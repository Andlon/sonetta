#include "spaudiooutput.h"

#include <QAtomicInt>
#include <QEvent>
#include <QMutex>
#include <QAudioFormat>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QDebug>

#include "circularbuffer.h"


enum SpAudioEvent {
    StartPlaybackEvent = QEvent::User + 5132,
    PausePlaybackEvent,
    StopPlaybackEvent,
    ResetBufferEvent
};

namespace SonataPrivate {
class SpAudioOutputPrivate {

public:
    SpAudioOutputPrivate() : volume(1), position(0), buffer(0), allowRead(1) { }

    qreal volume;
    QAtomicInt position;
    Spotify::PlaybackState state;
    CircularAudioBuffer * buffer;
    QAudioFormat format;

    QAtomicInt allowRead;

    QMutex volumeLock;
    QMutex bufferLock;
    QMutex stateLock;
};
}
SpAudioOutput::SpAudioOutput(QObject *parent) :
    QObject(parent), d(new SonataPrivate::SpAudioOutputPrivate)
{

    // Initialize to default spotify values
    d->format.setSampleRate(44100);
    d->format.setSampleSize(16);
    d->format.setSampleType(QAudioFormat::SignedInt);
    d->format.setChannelCount(2);
    d->format.setCodec("audio/pcm");
    d->format.setByteOrder(QAudioFormat::LittleEndian);

    d->buffer = new CircularAudioBuffer(22500, 2048, this);
    d->buffer->open(QIODevice::ReadWrite);

    d->state = Spotify::StoppedState;
}

SpAudioOutput::~SpAudioOutput()
{
    d->buffer->close();
    delete d->buffer;
    delete d;
}

Spotify::PlaybackState SpAudioOutput::state() const
{
    QMutexLocker locker(&d->stateLock);
    return d->state;
}

int SpAudioOutput::deliverAudio(const QAudioFormat &format, const void *frames, int numFrames)
{
    if (numFrames == 0)
    {
        reset();
        return 0;
    }

    AudioFrameCollection collection(frames, numFrames, format);

    d->bufferLock.lock();
    int writtenFrames = d->buffer->writeFrames(collection);
    d->bufferLock.unlock();

    return writtenFrames;
}

void SpAudioOutput::updateVolume(qreal value)
{
    qreal realValue = updateVol(value);
    d->volumeLock.lock();
    d->volume = realValue;
    d->volumeLock.unlock();
    emit volumeUpdated(realValue);
}

qreal SpAudioOutput::updateVol(qreal)
{
    return 1;
}

void SpAudioOutput::updatePosition(int increment)
{
    d->position.fetchAndAddOrdered(increment);
}

int SpAudioOutput::position() const
{
    return d->position.load();
}

void SpAudioOutput::resetPosition(int pos)
{
    d->position.store(pos);
}

void SpAudioOutput::reset()
{
    d->allowRead.ref();
    // Clear internal buffer
    d->buffer->reset();

    QEvent * event = new QEvent((QEvent::Type) ResetBufferEvent);
    QCoreApplication::postEvent(this, event);
}

void SpAudioOutput::play()
{
    if (state() == Spotify::PlayState)
        return;

    d->allowRead.ref();
    d->stateLock.lock();
    d->state = Spotify::PlayState;
    d->stateLock.unlock();
    emit stateChanged();

    QEvent * event = new QEvent((QEvent::Type) StartPlaybackEvent);
    QCoreApplication::postEvent(this, event);
}

void SpAudioOutput::pause()
{
    if (state() == Spotify::PausedState)
        return;

    d->allowRead.ref();
    d->stateLock.lock();
    d->state = Spotify::PausedState;
    d->stateLock.unlock();
    emit stateChanged();

    QEvent * event = new QEvent((QEvent::Type) PausePlaybackEvent);
    QCoreApplication::postEvent(this, event);
}

void SpAudioOutput::stop()
{
    if (state() == Spotify::PausedState)
        return;

    d->allowRead.ref();
    d->stateLock.lock();
    d->state = Spotify::StoppedState;
    d->stateLock.unlock();
    emit stateChanged();

    QEvent * event = new QEvent((QEvent::Type) StopPlaybackEvent);
    QCoreApplication::postEvent(this, event);
}

bool SpAudioOutput::event(QEvent *event)
{
    switch ((int) event->type())
    {
    case (int) StartPlaybackEvent:
        d->allowRead.deref();
        startAudio();
        return true;
        break;
    case (int) PausePlaybackEvent:
        pauseAudio();
        d->allowRead.deref();
        return true;
        break;
    case (int) StopPlaybackEvent:
        stopAudio();
        reset();
        d->allowRead.deref();
        return true;
        break;
    case (int) ResetBufferEvent:
        resetBuffer();
        d->allowRead.deref();
        return true;
        break;

    default:
        return QObject::event(event);
    }
}

qint64 SpAudioOutput::available() const
{
    bool allowRead = (bool) d->allowRead.load();
    return allowRead ? d->buffer->bytesAvailable() : 0;
}

qint64 SpAudioOutput::bytesInBuffer() const
{
    return d->buffer->bytesAvailable();
}

qint64 SpAudioOutput::read(char * data, qint64 maxLen)
{
    bool allowRead = (bool) d->allowRead.load();
    return allowRead ? d->buffer->read(data, maxLen) : 0;
}

QAudioFormat SpAudioOutput::format() const
{
   return d->buffer->format();
}
