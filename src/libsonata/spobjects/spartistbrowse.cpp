#include <Sonata/spartistbrowse.h>

#include "../spfactory.h"
#include "../spcore.h"

using namespace SonataPrivate;

SpArtistBrowse::SpArtistBrowse()
    :   d(0)
{
}

SpArtistBrowse::SpArtistBrowse(SpArtistBrowsePrivate *priv)
    :   d(priv)
{
}

SpArtistBrowse::SpArtistBrowse(const SpArtistBrowse &other)
{
    if (other.isValid())
    {
        d = new SpArtistBrowsePrivate(*other.d);
        sp_artistbrowse_add_ref(d->handle);
    }
    else
        d = 0;
}

SpArtistBrowse::~SpArtistBrowse()
{
    if (isValid())
        sp_artistbrowse_release(d->handle);

    delete d;
}

SpArtistBrowse& SpArtistBrowse::operator=(const SpArtistBrowse &other)
{
    if (other.isValid())
        sp_artistbrowse_add_ref(other.d->handle);

    if (isValid())
        sp_artistbrowse_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpArtistBrowsePrivate(*other.d) : 0;
    }

    return *this;
}

bool SpArtistBrowse::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpArtistBrowse::isLoaded() const
{
    return isValid() ? sp_artistbrowse_is_loaded(d->handle) : false;
}

SpArtist SpArtistBrowse::artist() const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(sp_artistbrowse_artist(d->handle));
    }

    return SpArtist();
}

QString SpArtistBrowse::biography() const
{
    if (isValid())
        return QString::fromUtf8(sp_artistbrowse_biography(d->handle));
    return QString();
}

int SpArtistBrowse::portraitCount() const
{
    return isValid() ? sp_artistbrowse_num_portraits(d->handle) : 0;
}

SpImage SpArtistBrowse::portraitAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createImageFromId(sp_artistbrowse_portrait(d->handle, index));
    }

    return SpImage();
}

SpImageList SpArtistBrowse::portraits() const
{
    SpImageList images;
    int count = portraitCount();

    for (int i = 0; i < count; ++i)
        images << portraitAt(i);

    return images;
}

int SpArtistBrowse::trackCount() const
{
    return isValid() ? sp_artistbrowse_num_tracks(d->handle) : 0;
}

SpTrack SpArtistBrowse::trackAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(sp_artistbrowse_track(d->handle, index));
    }

    return SpTrack();
}

SpTrackList SpArtistBrowse::tracks() const
{
    SpTrackList tracks;
    int count = trackCount();

    for (int i = 0; i < count; ++i)
        tracks << trackAt(i);

    return tracks;
}

int SpArtistBrowse::albumCount() const
{
    return isValid() ? sp_artistbrowse_num_albums(d->handle) : 0;
}

SpAlbum SpArtistBrowse::albumAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createAlbum(sp_artistbrowse_album(d->handle, index));
    }

    return SpAlbum();
}

SpAlbumList SpArtistBrowse::albums() const
{
    SpAlbumList albums;
    int count = albumCount();

    for (int i = 0; i < count; ++i)
        albums << albumAt(i);

    return albums;
}

int SpArtistBrowse::similarArtistCount() const
{
    return isValid() ? sp_artistbrowse_num_similar_artists(d->handle) : 0;
}

SpArtist SpArtistBrowse::similarArtistAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(sp_artistbrowse_similar_artist(d->handle, index));
    }

    return SpArtist();
}

SpArtistList SpArtistBrowse::similarArtists() const
{
    SpArtistList artists;
    int count = similarArtistCount();

    for (int i = 0; i < count; ++i)
        artists << similarArtistAt(i);

    return artists;
}

int SpArtistBrowse::topTrackCount() const
{
    return isValid() ? sp_artistbrowse_num_tophit_tracks(d->handle) : 0;
}

SpTrack SpArtistBrowse::topTrackAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(sp_artistbrowse_tophit_track(d->handle, index));
    }

    return SpTrack();
}

SpTrackList SpArtistBrowse::topTracks() const
{
    SpTrackList tracks;
    int count = topTrackCount();

    for (int i = 0; i < count; ++i)
        tracks << topTrackAt(i);

    return tracks;
}

bool operator==(const SpArtistBrowse &browse1, const SpArtistBrowse &browse2)
{
    if (!browse1.isValid() || !browse2.isValid())
        return false;

    return browse1.d->handle == browse2.d->handle;
}


