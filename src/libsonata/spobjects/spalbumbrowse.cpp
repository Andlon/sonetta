#include <Sonata/spalbumbrowse.h>

#include "../spfactory.h"
#include "../spcore.h"

using namespace SonataPrivate;

SpAlbumBrowse::SpAlbumBrowse()
    :   d(0)
{
}

SpAlbumBrowse::SpAlbumBrowse(SpAlbumBrowsePrivate *priv)
    :   d(priv)
{
}

SpAlbumBrowse::SpAlbumBrowse(const SpAlbumBrowse &other)
{
    if (other.isValid())
    {
        d = new SpAlbumBrowsePrivate(*other.d);
        sp_albumbrowse_add_ref(d->handle);
    }
    else
        d = 0;
}

SpAlbumBrowse::~SpAlbumBrowse()
{
    if (isValid())
        sp_albumbrowse_release(d->handle);

    delete d;
}

SpAlbumBrowse& SpAlbumBrowse::operator=(const SpAlbumBrowse &other)
{
    if (other.isValid())
        sp_albumbrowse_add_ref(other.d->handle);

    if (isValid())
        sp_albumbrowse_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpAlbumBrowsePrivate(*other.d) : 0;
    }

    return *this;
}

bool SpAlbumBrowse::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpAlbumBrowse::isLoaded() const
{
    return isValid() ? sp_albumbrowse_is_loaded(d->handle) : false;
}

SpArtist SpAlbumBrowse::artist() const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(sp_albumbrowse_artist(d->handle));
    }

    return SpArtist();
}

QString SpAlbumBrowse::review() const
{
    return isValid() ? sp_albumbrowse_review(d->handle) : QString();
}

int SpAlbumBrowse::trackCount() const
{
    return isValid() ? sp_albumbrowse_num_tracks(d->handle) : 0;
}

SpTrack SpAlbumBrowse::trackAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(sp_albumbrowse_track(d->handle, index));
    }

    return SpTrack();
}

SpTrackList SpAlbumBrowse::tracks() const
{
    SpTrackList tracks;
    int count = trackCount();

    for (int i = 0; i < count; ++i)
        tracks << trackAt(i);

    return tracks;
}


bool operator==(const SpAlbumBrowse &browse1, const SpAlbumBrowse &browse2)
{
    if (!browse1.isValid() || !browse2.isValid())
        return false;

    return browse1.d->handle == browse2.d->handle;
}

