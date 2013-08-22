#include "spdefaultoutput.h"

#include <QAudioOutput>
#include <QIODevice>
#include <QTimer>
#include <QDebug>

static const int BUFFERSIZE = 16384;
static const int PLAYBACKTRESHOLD = BUFFERSIZE;

namespace SonataPrivate {
class SpDefaultOutputPrivate {
public:
    SpDefaultOutputPrivate() : output(0), device(0), lastPosition(0), offset(0), delayPlayback(false), bufferBytes(0) { }

    QAudioOutput * output;
    QIODevice * device;
    QTimer pushTimer;

    int lastPosition;
    int offset;

    bool delayPlayback;

    char * buffer;
    int bufferBytes;
};
}


SpDefaultOutput::SpDefaultOutput(QObject *parent) :
    SpAudioOutput(parent), d(new SonataPrivate::SpDefaultOutputPrivate)
{
    d->pushTimer.setParent(this);
    d->pushTimer.setSingleShot(false);

    d->buffer = new char[BUFFERSIZE];

    connect(&d->pushTimer, SIGNAL(timeout()), this, SLOT(push()));
}

SpDefaultOutput::~SpDefaultOutput()
{
    delete[] d->buffer;
    delete d;
}

void SpDefaultOutput::resetBuffer()
{
    if (d->output)
    {
        d->output->reset();
        if (state() == Spotify::PlayState)
            startAudio();
    }

    d->delayPlayback = true;
}

void SpDefaultOutput::push()
{
    if (d->output && d->device)
    {
        int available = this->available();
        if (d->delayPlayback && available + d->bufferBytes < PLAYBACKTRESHOLD)
            return;
        else
            d->delayPlayback = false;

        qint64 periodSize = d->output->periodSize();

        while (periodSize < available + d->bufferBytes)
        {
            int freeBytes = d->output->bytesFree();
            if (freeBytes < periodSize)
                break;

            QAudioFormat format = this->format();
            if (format != d->output->format())
            {
                if (d->output->bytesFree() == d->output->bufferSize())
                {
                    // Recreate output with correct format
                    d->output->stop();
                    d->output->deleteLater();
                    d->output = 0;
                    d->bufferBytes = 0;
                    startAudio();
                    return;
                }
                else
                    return;
            }

            // Re-use buffer to prevent having to frequently reallocate memory
            if (freeBytes > d->bufferBytes)
            {
                qint64 maxRead = qMin<int>(freeBytes - d->bufferBytes, BUFFERSIZE);
                d->bufferBytes += read(d->buffer, maxRead);
            }
            qint64 toWrite = qMin<int>(freeBytes, d->bufferBytes);
            qint64 written = d->device->write(d->buffer, toWrite);
            d->bufferBytes -= written;

            if (written < toWrite)
                qDebug() << "Could not write all bytes to device. Extra bytes: " << toWrite - written;

            // Calculate new position
            int position = d->output->processedUSecs() / 1000 - d->offset;
            updatePosition(position - d->lastPosition);
            d->lastPosition = position;
        }

        if (periodSize >= available + d->bufferBytes)
        {
            // Place into temporary buffer
            d->bufferBytes += read(d->buffer + d->bufferBytes, BUFFERSIZE - d->bufferBytes);
        }
    }
}

void SpDefaultOutput::startAudio()
{
    if (d->output == 0)
    {
        d->output = new QAudioOutput(format(), this);
        connect(d->output, SIGNAL(stateChanged(QAudio::State)),
                this, SLOT(handleStateChange(QAudio::State)));
    }

    if (d->output->state() == QAudio::ActiveState || d->output->state() == QAudio::IdleState)
        return;
    else if (d->output->state() == QAudio::SuspendedState)
        d->output->resume();
    else if (d->output->state() == QAudio::StoppedState)
    {
        d->device = d->output->start();
        d->offset = 0;
        d->lastPosition = 0;
        d->bufferBytes = 0;
        d->delayPlayback = true;
    }


    // Determine timer interval (msecs)
    qint64 period = format().durationForBytes(d->output->periodSize()) / 1000;
    // Update with quarted period
    d->pushTimer.start(period / 4);
}

void SpDefaultOutput::pauseAudio()
{
    if (d->output)
    {
        if (d->output->state() == QAudio::ActiveState || d->output->state() == QAudio::IdleState)
        {
            d->output->suspend();
            d->pushTimer.stop();
        }
    }
}

void SpDefaultOutput::stopAudio()
{
    if (d->output)
    {
        if (d->output->state() != QAudio::StoppedState)
        {
            d->output->stop();
            d->pushTimer.stop();
        }
    }
}

qreal SpDefaultOutput::updateVol(qreal volume)
{
    d->output->setVolume(volume);
    return d->output->volume();
}

void SpDefaultOutput::handleStateChange(QAudio::State state)
{
    if (state == QAudio::IdleState && d->output->error() == QAudio::UnderrunError)
        d->delayPlayback = true;
}
