#ifndef SPARTISTBROWSE_H
#define SPARTISTBROWSE_H

#include "definitions.h"

namespace SonataPrivate {
class SpArtistBrowsePrivate;
class SpFactory;
}

class SpArtistBrowse {
public:
    SpArtistBrowse();
    SpArtistBrowse(const SpArtistBrowse &other);
    ~SpArtistBrowse();

    bool isValid() const;
    bool isLoaded() const;

    SpArtist artist() const;
    QString biography() const;

    int portraitCount() const;
    SpImage portraitAt(int index) const;
    SpImageList portraits() const;

    // trackCount always 0? Possibly remove
    int trackCount() const;
    SpTrack trackAt(int index) const;
    SpTrackList tracks() const;

    int albumCount() const;
    SpAlbum albumAt(int index) const;
    SpAlbumList albums() const;

    int similarArtistCount() const;
    SpArtist similarArtistAt(int index) const;
    SpArtistList similarArtists() const;

    int topTrackCount() const;
    SpTrack topTrackAt(int index) const;
    SpTrackList topTracks() const;

    SpArtistBrowse& operator=(const SpArtistBrowse &other);

private:
    SonataPrivate::SpArtistBrowsePrivate * d;
    SpArtistBrowse(SonataPrivate::SpArtistBrowsePrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpArtistBrowse &browse1, const SpArtistBrowse &browse2);
};

Q_DECLARE_METATYPE(SpArtistBrowse)

#endif // SPARTISTBROWSE_H
