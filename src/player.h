#pragma once

#include <QObject>
#include <QQueue>
#include <QQuickView>
#include <QPointer>

#include <Spotinetta/Spotinetta>

#include "navigation/navigation.h"
#include "models/queuemodel.h"
#include "audiooutput.h"

class QTimer;

namespace Sonetta {

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(int position READ position NOTIFY positionChanged)
    Q_PROPERTY(Spotinetta::Track track READ track NOTIFY trackChanged)
    Q_PROPERTY(QObject * queue READ queue CONSTANT)

public:
    explicit Player(Spotinetta::Session * session, AudioOutput * output, QObject *parent = 0);

    bool shuffle() const;
    bool repeat() const;
    int position() const;
    Spotinetta::Track track() const;

    void setShuffle(bool enable);
    void setRepeat(bool enable);

    QObject * queue() const;
    
signals:
    void shuffleChanged();
    void repeatChanged();
    void trackChanged();
    void positionChanged();
    
public slots:
    void play(const Spotinetta::Track &track);
    void enqueue(const Spotinetta::Track &track);

    void play();
    void playPause();
    void pause();
    void next();

private:
    QPointer<Spotinetta::Session>   m_session;
    QPointer<AudioOutput>           m_output;
    Spotinetta::TrackWatcher *      m_watcher;
    QueueModel *                    m_queue;

    QTimer *                        m_positionTimer;

    bool m_shuffle;
    bool m_repeat;
};

}
