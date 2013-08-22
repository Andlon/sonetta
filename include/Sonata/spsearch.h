#ifndef SPSEARCH_H
#define SPSEARCH_H

#include "definitions.h"
#include <QMetaType>

class QString;

namespace SonataPrivate {
class SpSearchPrivate;
class SpFactory;
}

class SpSearch
{
public:
    SpSearch();
    SpSearch(const SpSearch &other);
    ~SpSearch();

    bool isValid() const;
    bool isLoaded() const;

    QString query() const;
    QString didYouMean() const;

    int trackCount() const;
    SpTrack trackAt(int index) const;
    SpTrackList tracks() const;

    int albumCount() const;
    SpAlbum albumAt(int index) const;
    SpAlbumList albums() const;

    int artistCount() const;
    SpArtist artistAt(int index) const;
    SpArtistList artists() const;

    int playlistCount() const;
    SpPlaylist playlistAt(int index) const;
    SpPlaylistList playlists() const;

    SpSearch& operator=(const SpSearch &other);

private:
    SonataPrivate::SpSearchPrivate * d;
    SpSearch(SonataPrivate::SpSearchPrivate *);

    friend class SonataPrivate::SpFactory;
    friend bool operator==(const SpSearch &t1, const SpSearch &t2);
};

bool operator==(const SpSearch &s1, const SpSearch &s2);

Q_DECLARE_METATYPE(SpSearch)

#endif // SPSEARCH_H
