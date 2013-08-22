#include <Sonata/spsearch.h>

#include "../spfactory.h"
#include "../spcore.h"


using namespace SonataPrivate;

SpSearch::SpSearch()
    :   d(0)
{
}

SpSearch::SpSearch(SpSearchPrivate * priv)
    :   d(priv)
{
    if (isValid())
        sp_search_add_ref(d->handle);
}

SpSearch::SpSearch(const SpSearch &other)
{
    if (other.isValid())
    {
        d = new SpSearchPrivate(*other.d);
        sp_search_add_ref(d->handle);
    }
    else
        d = 0;
}

SpSearch::~SpSearch()
{
    if (isValid())
        sp_search_release(d->handle);

    delete d;
}

SpSearch& SpSearch::operator=(const SpSearch &other)
{
    if (other.isValid())
        sp_search_add_ref(other.d->handle);

    if (isValid())
        sp_search_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpSearchPrivate(*other.d) : 0;
    }

    return *this;
}

bool SpSearch::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpSearch::isLoaded() const
{
    return isValid() ? sp_search_is_loaded(d->handle) : false;
}

QString SpSearch::query() const
{
    return isValid() ? QString::fromUtf8(sp_search_query(d->handle)) : QString();
}

QString SpSearch::didYouMean() const
{
    return isValid() ? QString::fromUtf8(sp_search_did_you_mean(d->handle)) : QString();
}

int SpSearch::trackCount() const
{
    return isValid() ? sp_search_num_tracks(d->handle) : 0;
}

int SpSearch::albumCount() const
{
    return isValid() ? sp_search_num_albums(d->handle) : 0;
}

int SpSearch::artistCount() const
{
    return isValid() ? sp_search_num_artists(d->handle) : 0;
}

int SpSearch::playlistCount() const
{
    return isValid() ? sp_search_num_playlists(d->handle) : 0;
}

SpTrack SpSearch::trackAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(sp_search_track(d->handle, index));
    }

    return SpTrack();
}

SpTrackList SpSearch::tracks() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return QList<SpTrack>();

    SpTrackList tracks;

    int count = trackCount();
    for (int i = 0; i < count; ++i)
        tracks << core->factory()->createTrack(sp_search_track(d->handle, i));

    return tracks;
}

SpAlbum SpSearch::albumAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createAlbum(sp_search_album(d->handle, index));
    }

    return SpAlbum();
}

SpAlbumList SpSearch::albums() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return QList<SpAlbum>();

    SpAlbumList albums;

    int count = albumCount();
    for (int i = 0; i < count; ++i)
        albums << core->factory()->createAlbum(sp_search_album(d->handle, i));

    return albums;
}

SpPlaylist SpSearch::playlistAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createPlaylist(sp_search_playlist(d->handle, index));
    }

    return SpPlaylist();
}

SpPlaylistList SpSearch::playlists() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return QList<SpPlaylist>();

    SpPlaylistList playlists;

    int count = playlistCount();
    for (int i = 0; i < count; ++i)
        playlists << core->factory()->createPlaylist(sp_search_playlist(d->handle, i));

    return playlists;
}


SpArtist SpSearch::artistAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(sp_search_artist(d->handle, index));
    }

    return SpArtist();
}

SpArtistList SpSearch::artists() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return QList<SpArtist>();

    SpArtistList artists;

    int count = artistCount();
    for (int i = 0; i < count; ++i)
        artists << core->factory()->createArtist(sp_search_artist(d->handle, i));

    return artists;
}



bool operator==(const SpSearch &s1, const SpSearch &s2)
{
    if (!s1.isValid() || !s2.isValid())
        return false;

    return s1.d->handle == s2.d->handle;
}
