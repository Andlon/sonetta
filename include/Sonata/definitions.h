#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QMetaType>
#include <QList>
#include "spotifyenum.h"

class SpTrack;
class SpArtist;
class SpAlbum;
class SpPlaylist;
class SpPlaylistContainer;
class SpImage;
class SpArtistBrowse;
class SpAlbumBrowse;
class SpSearch;

typedef QList<SpTrack> SpTrackList;
typedef QList<SpArtist> SpArtistList;
typedef QList<SpPlaylist> SpPlaylistList;
typedef QList<SpImage> SpImageList;
typedef QList<SpAlbum> SpAlbumList;



#endif // DEFINITIONS_H
