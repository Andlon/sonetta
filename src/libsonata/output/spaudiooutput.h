#ifndef SPAUDIOOUTPUT_H
#define SPAUDIOOUTPUT_H

#include <QObject>
#include <QAudioFormat>
#include <Sonata/Spotify>

namespace SonataPrivate {
class SpAudioOutputPrivate;
}

class SpAudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit SpAudioOutput(QObject *parent = 0);
    virtual ~SpAudioOutput();

    int deliverAudio(const QAudioFormat &format, const void * frames, int numFrames);
    void reset();

    Spotify::PlaybackState state() const;

    int position() const;
    qreal volume() const;

    qint64 bytesInBuffer() const;

    QAudioFormat format() const;

    void play();
    void pause();
    void stop();

    bool event(QEvent *);

    void resetPosition(int pos);

public slots:
    void updateVolume(qreal value);

signals:
    void stateChanged();
    void volumeUpdated(qreal volume);

protected:
    virtual void startAudio() = 0;
    virtual void stopAudio() = 0;
    virtual void pauseAudio() = 0;
    virtual void resetBuffer() = 0;

    virtual qreal updateVol(qreal volume);
    void updatePosition(int increment);

    qint64 available() const;

    qint64 read(char * data, qint64 maxLen);

private:
    SonataPrivate::SpAudioOutputPrivate * d;
};

#endif // SPAUDIOOUTPUT_H
