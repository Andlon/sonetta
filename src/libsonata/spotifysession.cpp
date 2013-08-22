#include <Sonata/spotifysession.h>

#include "spotifyevent.h"
#include "spcore.h"

#include "libspotify/api.h"

#include <stdint.h>

#include <QString>
#include <QMetaType>

#ifdef QT_QUICK_LIB
#include <QtQml>
#endif


using namespace SonataPrivate;
using namespace Sonata;

namespace SonataPrivate {
class SpotifyPrivate {
public:
    SpotifyPrivate(const uint8_t *key, size_t size) : core(SpCore::createHandle(key, size)) { }

    SpHandle core;
};
}

void Sonata::registerMetaTypes()
{
    qRegisterMetaType<SpTrack>();
    qRegisterMetaType<SpTrackList>();
    qRegisterMetaType<SpLink>();
    qRegisterMetaType<SpArtist>();
    qRegisterMetaType<SpArtistList>();
    qRegisterMetaType<SpPlaylistContainer>();
    qRegisterMetaType<SpPlaylist>();
    qRegisterMetaType<SpImage>();
    qRegisterMetaType<SpImageList>();
    qRegisterMetaType<SpArtistBrowse>();
    qRegisterMetaType<SpAlbum>();
    qRegisterMetaType<SpAlbumList>();
    qRegisterMetaType<SpAlbumBrowse>();
    qRegisterMetaType<SpSearch>();
}

SpotifySession::SpotifySession(const uint8_t *key, size_t size, QObject *parent)
    :   QObject(parent), d(new SonataPrivate::SpotifyPrivate(key, size))
{
    connect(d->core.data(), SIGNAL(loggedIn()), this, SIGNAL(loggedIn()));
    connect(d->core.data(), SIGNAL(loggedOut()), this, SIGNAL(loggedOut()));
    connect(d->core.data(), SIGNAL(connectionStateChanged()), this, SIGNAL(connectionStateChanged()));
    connect(d->core.data(), SIGNAL(playbackStateChanged()), this, SIGNAL(playbackStateChanged()));
    connect(d->core.data(), SIGNAL(positionUpdated()), this, SIGNAL(positionUpdated()));
    connect(d->core.data(), SIGNAL(endOfTrack()), this, SIGNAL(endOfTrack()));
    connect(d->core.data(), SIGNAL(currentTrackChanged()), this, SIGNAL(currentTrackChanged()));
    connect(d->core->factory(), SIGNAL(trackUpdated(SpTrack)), this, SIGNAL(trackUpdated(SpTrack)));

    connect(d->core.data(), SIGNAL(loggedIn()), this, SIGNAL(rootContainerChanged()));
    connect(d->core.data(), SIGNAL(loggedOut()), this, SIGNAL(rootContainerChanged()));
}

SpotifySession::~SpotifySession()
{
    delete d;
}

bool SpotifySession::createSession()
{
    return d->core->createSession();
}

void SpotifySession::login(const QString &username, const QString &password)
{
    d->core->login(username, password);
}

void SpotifySession::logout()
{
    d->core->logout();
}

QString SpotifySession::username() const
{
    // Fix
    return QString();
}

int SpotifySession::position() const
{
    return d->core->position();
}

Spotify::ConnectionState SpotifySession::connectionState() const
{
    return d->core->connectionState();
}

Spotify::PlaybackState SpotifySession::playbackState() const
{
    return d->core->playbackState();
}

bool SpotifySession::play(const SpTrack &track)
{
    return d->core->play(track);
}

bool SpotifySession::play()
{
    return d->core->play();
}

bool SpotifySession::pause()
{
    return d->core->pause();
}

bool SpotifySession::stop()
{
    return d->core->stop();
}

void SpotifySession::seek(int offset)
{
    d->core->seek(offset);
}

SpTrack SpotifySession::currentTrack() const
{
    return d->core->currentTrack();
}

SpPlaylistContainer SpotifySession::rootContainer() const
{
    return d->core->rootContainer();
}

SpArtistBrowse SpotifySession::browse(const SpArtist &artist, Spotify::ArtistBrowseType type) const
{
    return d->core->factory()->browse(artist, type);
}

SpAlbumBrowse SpotifySession::browse(const SpAlbum &album)
{
    return d->core->factory()->browse(album);
}

SpSearch SpotifySession::search(const QString &query, int trackOffset, int maxTracks, int albumOffset, int maxAlbums, int artistOffset, int maxArtists, int playlistOffset, int maxPlaylists, Spotify::SearchType type)
{
    return d->core->factory()->search(query, trackOffset, maxTracks,
                                      albumOffset, maxAlbums,
                                      artistOffset, maxArtists,
                                      playlistOffset, maxPlaylists,
                                      type);
}

