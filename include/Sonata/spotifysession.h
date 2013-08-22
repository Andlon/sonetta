#ifndef SPOTIFYSESSION_H
#define SPOTIFYSESSION_H

#include <QObject>

#include <stdint.h>

#ifdef Q_OS_LINUX
#include <stdint-gcc.h>
#endif

#include "definitions.h"

#include "splink.h"
#include "sptrack.h"
#include "spartist.h"
#include "spalbum.h"
#include "spplaylist.h"
#include "spplaylistcontainer.h"
#include "spimage.h"
#include "spartistbrowse.h"
#include "spalbumbrowse.h"
#include "spsearch.h"

class QString;

namespace SonataPrivate {
class SpotifyPrivate;
}

class SpotifySession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Spotify::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(Spotify::PlaybackState playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(int position READ position NOTIFY positionUpdated)
    Q_PROPERTY(SpTrack currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(SpPlaylistContainer rootContainer READ rootContainer NOTIFY rootContainerChanged)
public:
    explicit SpotifySession(const uint8_t * key, size_t size, QObject * parent = 0);
    ~SpotifySession();

    bool createSession();

    QString username() const;

    Spotify::ConnectionState connectionState() const;
    Spotify::PlaybackState playbackState() const;
    int position() const;
    SpTrack currentTrack() const;

    SpPlaylistContainer rootContainer() const;

    Q_INVOKABLE SpArtistBrowse browse(const SpArtist &artist, Spotify::ArtistBrowseType type = Spotify::ArtistBrowseNoTracks) const;
    Q_INVOKABLE SpAlbumBrowse browse(const SpAlbum &album);
    Q_INVOKABLE SpSearch search(const QString &query, int trackOffset, int maxTracks,
                                int albumOffset, int maxAlbums,
                                int artistOffset, int maxArtists,
                                int playlistOffset, int maxPlaylists,
                                Spotify::SearchType type);

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
    void rootContainerChanged();

    // Metadata updates
    void trackUpdated(SpTrack track);
    void artistUpdated(SpArtist artist);

private:
    Q_DISABLE_COPY(SpotifySession)
    SonataPrivate::SpotifyPrivate * d;
};

namespace Sonata {
void registerMetaTypes();
}

#endif // SPOTIFYSESSION_H
