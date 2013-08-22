#include <Spotinetta/search.h>
#include <Spotinetta/track.h>
#include <Spotinetta/album.h>
#include <Spotinetta/artist.h>

namespace Spotinetta {

QString Search::query() const
{
    return isValid() ? QString::fromUtf8(sp_search_query(handle())) : QString();
}

QString Search::didYouMean() const
{
    return isValid() ? QString::fromUtf8(sp_search_did_you_mean(handle())) : QString();
}

int Search::trackCount() const
{
    return isValid() ? sp_search_num_tracks(handle()) : 0;
}

int Search::albumCount() const
{
    return isValid() ? sp_search_num_albums(handle()) : 0;
}

int Search::artistCount() const
{
    return isValid() ? sp_search_num_artists(handle()) : 0;
}

int Search::playlistCount() const
{
    return isValid() ? sp_search_num_playlists(handle()) : 0;
}

Track Search::trackAt(int index) const
{
    return isValid() ? sp_search_track(handle(), index) : Track();
}

TrackList Search::tracks() const
{
    TrackList list;
    for (int i = 0; i < trackCount(); ++i)
        list << trackAt(i);

    return list;
}

Album Search::albumAt(int index) const
{
    return isValid() ? sp_search_album(handle(), index) : Album();
}

AlbumList Search::albums() const
{
    AlbumList list;
    for (int i = 0; i < albumCount(); ++i)
        list << albumAt(i);

    return list;
}

Artist Search::artistAt(int index) const
{
    return isValid() ? sp_search_artist(handle(), index) : Artist();
}

ArtistList Search::artists() const
{
    ArtistList list;
    for (int i = 0; i < artistCount(); ++i)
        list << artistAt(i);

    return list;
}

}
