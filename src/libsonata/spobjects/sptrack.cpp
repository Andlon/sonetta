#include <Sonata/sptrack.h>

#include "../spfactory.h"
#include "../spcore.h"

using namespace SonataPrivate;

SpTrack::SpTrack()
    :   d(0)
{
}

SpTrack::SpTrack(SpTrackPrivate * priv)
    :   d(priv)
{
    if (isValid())
        sp_track_add_ref(d->handle);
}

SpTrack::SpTrack(const SpTrack &other)
{
    if (other.isValid())
    {
        d = new SpTrackPrivate(*other.d);
        sp_track_add_ref(d->handle);
    }
    else
        d = 0;
}

SpTrack::~SpTrack()
{
    if (isValid())
        sp_track_release(d->handle);

    delete d;
}

SpTrack& SpTrack::operator=(const SpTrack &other)
{
    if (other.isValid())
        sp_track_add_ref(other.d->handle);

    if (isValid())
        sp_track_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpTrackPrivate(*other.d) : 0;
    }

    return *this;
}

SpArtistList SpTrack::artists() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return QList<SpArtist>();

    SpArtistList artists;

    int count = artistCount();
    for (int i = 0; i < count; ++i)
        artists << core->factory()->createArtist(sp_track_artist(d->handle, i));

    return artists;
}

SpArtist SpTrack::artistAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(sp_track_artist(d->handle, index));
    }

    return SpArtist();
}

SpAlbum SpTrack::album() const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createAlbum(sp_track_album(d->handle));
    }

    return SpAlbum();
}


bool SpTrack::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpTrack::isLoaded() const
{
    return isValid() ? sp_track_is_loaded(d->handle) : false;
}

bool SpTrack::isAutoLinked() const
{
    return isValid() ? sp_track_is_autolinked(d->session.data(), d->handle) : false;
}

bool SpTrack::isAvailable() const
{
    return isValid() ? sp_track_get_availability(d->session.data(), d->handle) == SP_TRACK_AVAILABILITY_AVAILABLE : false;
}

QString SpTrack::name() const
{
    return isValid() ? QString::fromUtf8(sp_track_name(d->handle)) : QString();
}

int SpTrack::duration() const
{
    return isValid() ? sp_track_duration(d->handle) : 0;
}

int SpTrack::popularity() const
{
    return isValid() ? sp_track_popularity(d->handle) : 0;
}

int SpTrack::artistCount() const
{
    return isValid()? sp_track_num_artists(d->handle) : 0;
}

SpTrack SpTrack::playableTrack() const
{
    SpHandle core = SpCore::handle();
    if (isValid() && !core.isNull())
        return core->factory()->createTrack(sp_track_get_playable(d->session.data(), d->handle));

    return SpTrack();
}

bool operator==(const SpTrack &t1, const SpTrack &t2)
{
    if (!t1.isValid() || !t2.isValid())
        return false;

    return t1.d->handle == t2.d->handle;
}
