#ifndef SPCORE_H
#define SPCORE_H

#include <QObject>
#include <QWeakPointer>
#include <QSharedPointer>
#include <QTimer>
#include <QThread>

#include <Sonata/spotifysession.h>

#include <libspotify/api.h>

#include "output/spaudiooutput.h"
#include "spfactory.h"
#include "spotifyevent.h"

namespace SonataPrivate {

class SpCore;
typedef QSharedPointer<SpCore> SpHandle;
typedef QWeakPointer<sp_session> SessionHandle;

class SpCore : public QObject
{
    Q_OBJECT

    typedef Spotify::ConnectionState ConnectionState;
    typedef Spotify::PlaybackState PlaybackState;
public:
    ~SpCore();

    static SpHandle createHandle(const uint8_t * key, size_t size);
    static SpHandle handle();

    bool createSession();
    void releaseSession();

    bool sessionIsValid() const;
    SessionHandle session() const;
    ConnectionState connectionState() const;
    PlaybackState playbackState() const;

    SpFactory * factory() const;
    SpAudioOutput * audioOutput() const;

    SpTrack currentTrack() const;

    SpPlaylistContainer rootContainer() const;

    int position() const;

    void setAudioOutput(SpAudioOutput * output);

    bool event(QEvent *);

public slots:
    void login(const QString &username, const QString &password);
    void logout();

    bool play(const SpTrack &track);

    bool play();
    bool pause();
    bool stop();
    void seek(int offset);

signals:
    void loggedIn();
    void loggedOut();
    void connectionStateChanged();
    void playbackStateChanged();
    void positionUpdated();
    void endOfTrack();
    void currentTrackChanged();

protected:
    bool spotifyEvent(SpotifyEvent *event);

private slots:
    void handlePolling();

private:
    explicit SpCore(const uint8_t * key, size_t size);
    void processSpotifyEvents();
    void setupOutputConnections();
    void processEndOfTrack();

    // Application key
    uint8_t *   m_key;
    size_t      m_keySize;

    // Spotify objects
    QSharedPointer<sp_session> m_session;

    // Misc
    ConnectionState m_state;
    SpAudioOutput * m_output;
    SpFactory * m_factory;
    int m_timerId;
    QThread * m_audioThread;
    QTimer * m_pollTimer;

    QString m_username;

    SpTrack m_currentTrack;
    SpPlaylistContainer m_rootContainer;

    static QWeakPointer<SpCore> s_instance;

    friend class SpFactory;
    
};

}

#endif // SPCORE_H
