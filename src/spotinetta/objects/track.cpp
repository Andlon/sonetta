#include <Spotinetta/track.h>
#include <Spotinetta/artist.h>
#include <Spotinetta/album.h>

namespace Spotinetta {

QString Track::name() const
{
    return isValid() ? QString::fromUtf8(sp_track_name(handle())) : QString();
}

int Track::duration() const
{
    return isValid() ? sp_track_duration(handle()) : 0;
}

int Track::popularity() const
{
    return isValid() ? sp_track_popularity(handle()) : 0;
}

int Track::artistCount() const
{
    return isValid()? sp_track_num_artists(handle()) : 0;
}

Artist Track::artistAt(int index) const
{
    return isValid() ? Artist(sp_track_artist(handle(), index)) : Artist();
}

ArtistList Track::artists() const
{
    ArtistList list;
    for (int i = 0; i < artistCount(); ++i)
        list << artistAt(i);

    return list;
}

Album Track::album() const
{
    return isValid() ? Album(sp_track_album(handle())) : Album();
}

}
