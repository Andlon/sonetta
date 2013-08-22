#pragma once

#include <QMetaType>
#include <QList>
#include <QString>
#include <libspotify/api.h>

namespace Spotinetta {

class Track;
class Album;
class Artist;
class Playlist;
class PlaylistContainer;
class Search;
class ArtistBrowse;
class AlbumBrowse;
class Image;
class Session;

typedef QList<Track>    TrackList;
typedef QList<Album>    AlbumList;
typedef QList<Artist>   ArtistList;
typedef QList<Playlist> PlaylistList;
typedef QList<Search>   SearchList;
typedef QList<Image>    ImageList;

}
