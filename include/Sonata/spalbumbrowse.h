#ifndef SPALBUMBROWSE_H
#define SPALBUMBROWSE_H

#include "definitions.h"

namespace SonataPrivate {
class SpAlbumBrowsePrivate;
class SpFactory;
}

class SpAlbumBrowse {
public:
    SpAlbumBrowse();
    SpAlbumBrowse(const SpAlbumBrowse &other);
    ~SpAlbumBrowse();

    bool isValid() const;
    bool isLoaded() const;

    SpAlbum album() const;
    SpArtist artist() const;

    QString review() const;

    int trackCount() const;
    SpTrack trackAt(int index) const;
    SpTrackList tracks() const;

    SpAlbumBrowse& operator=(const SpAlbumBrowse &other);

private:
    SonataPrivate::SpAlbumBrowsePrivate * d;
    SpAlbumBrowse(SonataPrivate::SpAlbumBrowsePrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpAlbumBrowse &browse1, const SpAlbumBrowse &browse2);
};

Q_DECLARE_METATYPE(SpAlbumBrowse)

#endif // SPALBUMBROWSE_H
