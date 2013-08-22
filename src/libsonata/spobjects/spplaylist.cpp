#include <Sonata/spplaylist.h>
#include <Sonata/spimage.h>

#include "../spcore.h"
#include "../spfactory.h"

#include <QDebug>

using namespace SonataPrivate;

SpPlaylist::SpPlaylist()
    :   d(0)
{
}

SpPlaylist::SpPlaylist(SpPlaylistPrivate *priv)
    :   d(priv)
{
    if (isValid())
        sp_playlist_add_ref(d->handle);
}

SpPlaylist::SpPlaylist(const SpPlaylist &other)
{
    if (other.isValid())
    {
        d = new SpPlaylistPrivate(*other.d);
        sp_playlist_add_ref(d->handle);
    }
    else
        d = 0;
}

SpPlaylist::~SpPlaylist()
{
    if (isValid())
        sp_playlist_release(d->handle);

    delete d;
}

SpPlaylist& SpPlaylist::operator=(const SpPlaylist& other)
{
    if (other.isValid())
        sp_playlist_add_ref(other.d->handle);

    if (isValid())
        sp_playlist_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpPlaylistPrivate(*other.d) : 0;
    }

    return *this;
}

bool operator==(const SpPlaylist &p1, const SpPlaylist &p2)
{
    if (!p1.isValid() || !p2.isValid())
        return false;

    return p1.d->handle == p2.d->handle;
}

bool SpPlaylist::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpPlaylist::isLoaded() const
{
    return isValid() ? sp_playlist_is_loaded(d->handle) : false;
}

QString SpPlaylist::name() const
{
    return isValid() ? QString::fromUtf8(sp_playlist_name(d->handle)) : QString();
}

QString SpPlaylist::description() const
{
    return isValid() ? QString::fromUtf8(sp_playlist_get_description(d->handle)) : QString();
}

int SpPlaylist::trackCount() const
{
    return isValid() ? sp_playlist_num_tracks(d->handle) : 0;
}

SpTrackList SpPlaylist::tracks() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return SpTrackList();

    SpTrackList tracks;

    int count = trackCount();
    for (int i = 0; i < count; ++i)
        tracks << core->factory()->createTrack(sp_playlist_track(d->handle, i));

    return tracks;
}

SpTrack SpPlaylist::trackAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(sp_playlist_track(d->handle, index));
    }

    return SpTrack();
}

SpImage SpPlaylist::image() const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
        {
            byte id[20];
            if (sp_playlist_get_image(d->handle, id))
                return core->factory()->createImageFromId(&id[0]);
        }
    }

    return SpImage();
}
